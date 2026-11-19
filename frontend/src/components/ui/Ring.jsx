export default function Ring({ percent }) {
  const r = 38, c = 2 * Math.PI * r;
  return (
    <svg viewBox="0 0 100 100" className="ring" role="img" aria-label={`${percent}% progress`}>
      <circle cx="50" cy="50" r={r} className="ring-bg" />
      <circle cx="50" cy="50" r={r} className="ring-fg" strokeDasharray={c} strokeDashoffset={c * (1 - percent / 100)} transform="rotate(-90 50 50)" />
      <text x="50" y="55" textAnchor="middle" className="ring-text">{percent}%</text>
    </svg>
  );
}
