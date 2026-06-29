window.MathJax = {
  tex: {
    inlineMath: [['$', '$'], ['\\(', '\\)']],
    displayMath: [['$$', '$$'], ['\\[', '\\]']],
    processEscapes: true,
    processEnvironments: true
  },
  options: {
    // Configuration de ciblage pour le plugin mkdocs-jupyter
    ignoreHtmlClass: 'tex2jax_ignore',
    processHtmlClass: 'tex2jax_process|rendered_html'
  }
};

// Force le moteur de rendu graphique à s'exécuter une fois le DOM chargé
document.addEventListener('DOMContentLoaded', function() {
  if (window.MathJax && MathJax.typesetPromise) {
    MathJax.typesetPromise();
  }
});