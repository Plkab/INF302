window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\(', '\\)']],
    displayMath: [['$$', '$$'], ['\\[', '\\]']],
    processEscapes: true,
    processEnvironments: true
  },
  options: {
    ignoreHtmlClass: '.*|',
    processHtmlClass: '.*'
  }
};

// Forcer le rendu après le chargement de la page
document.addEventListener('DOMContentLoaded', function() {
  if (window.MathJax && MathJax.typesetPromise) {
    MathJax.typesetPromise();
  }
});