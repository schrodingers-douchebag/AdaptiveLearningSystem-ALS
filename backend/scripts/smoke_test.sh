#!/usr/bin/env bash
# End-to-end check against a RUNNING server:  ./scripts/smoke_test.sh [base-url]
BASE=${1:-http://localhost:8080}
j() { python3 -c "import sys,json; d=json.load(sys.stdin); print(eval(sys.argv[1]))" "$1"; }
say() { printf '\n== %s\n' "$1"; }

say "health";                 curl -s $BASE/api/health
say "register (weak pw)";     curl -s -X POST $BASE/api/auth/register -H 'Content-Type: application/json' -d '{"username":"zed","email":"zed@example.com","password":"short"}'
say "register";               curl -s -X POST $BASE/api/auth/register -H 'Content-Type: application/json' -d '{"username":"zed","email":"zed@example.com","password":"Zedpass123"}' | head -c 160; echo
TOKEN=$(curl -s -X POST $BASE/api/auth/login -H 'Content-Type: application/json' -d '{"username":"zed","password":"Zedpass123"}' | j "d['token']")
say "no token -> 401";        curl -s $BASE/api/questions
say "profile";                curl -s $BASE/api/users/profile -H "Authorization: Bearer $TOKEN"
say "start";                  curl -s -X POST $BASE/api/learning/start -H "Authorization: Bearer $TOKEN" | head -c 300; echo

say "answer 8 questions (student always picks option 1)"
QID=$(curl -s -X POST $BASE/api/learning/start -H "Authorization: Bearer $TOKEN" | j "d['question']['id']")
for i in 1 2 3 4 5 6 7 8; do
  Q=$(curl -s $BASE/api/questions/$QID -H "Authorization: Bearer $TOKEN")
  OPT=$(echo "$Q" | j "d['question']['options'][0]['id']")
  R=$(curl -s -X POST $BASE/api/learning/submit-answer -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' -d "{\"question_id\":$QID,\"selected_option_id\":$OPT,\"time_taken\":12}")
  echo "$R" | j "'q%d topic=%s correct=%s mastery=%.2f level %d->%d next=%s' % ($QID, d['topic'], d['is_correct'], d['updated_mastery'], d['previous_difficulty'], d['next_difficulty'], d['next_recommendation'])"
  QID=$(echo "$R" | j "d['next_recommendation']")
done
say "bad option id -> 400";   curl -s -X POST $BASE/api/learning/submit-answer -H "Authorization: Bearer $TOKEN" -H 'Content-Type: application/json' -d '{"question_id":1,"selected_option_id":9999}'
say "progress";               curl -s $BASE/api/learning/progress -H "Authorization: Bearer $TOKEN" | head -c 400; echo
say "weak topics";            curl -s $BASE/api/analytics/weak-topics -H "Authorization: Bearer $TOKEN" | head -c 300; echo
say "performance";            curl -s $BASE/api/analytics/performance -H "Authorization: Bearer $TOKEN" | head -c 300; echo
say "student -> teacher route (403)"; curl -s $BASE/api/analytics/students -H "Authorization: Bearer $TOKEN"
TT=$(curl -s -X POST $BASE/api/auth/login -H 'Content-Type: application/json' -d '{"username":"teacher","password":"Teacher#123"}' | j "d['token']")
say "teacher overview";       curl -s $BASE/api/analytics/students -H "Authorization: Bearer $TT" | head -c 400; echo
say "CORS preflight";         curl -s -i -X OPTIONS $BASE/api/questions -H 'Origin: http://localhost:5173' -H 'Access-Control-Request-Method: GET' | head -8
