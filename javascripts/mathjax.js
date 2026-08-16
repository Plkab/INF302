// ============================================================
// CONFIGURATION MATHJAX v3 - Version stable pour MkDocs + Jupyter
// ============================================================

window.MathJax = {
  tex: {
    inlineMath: [
      ['$', '$'],
      ['\\(', '\\)']
    ],
    displayMath: [
      ['$$', '$$'],
      ['\\[', '\\]']
    ],
    processEscapes: true,          // Permet d'échapper les $ avec \$
    processEnvironments: true      // Active les environnements comme \begin{equation}
  },
  svg: {
    fontCache: 'global'            // Optimise le cache des polices pour les pages longues
  },
  options: {
    // 🔥 SUPPRESSION DES FILTRES : on analyse TOUT le document
    // Cela garantit que le contenu des notebooks (.ipynb) est bien traité
    ignoreHtmlClass: '',
    processHtmlClass: ''
  }
};

// ============================================================
// DÉCLENCHEMENT DU RENDU APRÈS CHARGEMENT COMPLET DE MATHJAX
// ============================================================

if (document.readyState === 'complete') {
  // Si la page est déjà chargée, on lance le rendu immédiatement
  if (window.MathJax && MathJax.startup) {
    MathJax.startup.promise.then(() => {
      MathJax.typesetPromise();
    });
  }
} else {
  // Sinon, on attend que le DOM et MathJax soient prêts
  document.addEventListener('readystatechange', () => {
    if (document.readyState === 'complete') {
      if (window.MathJax && MathJax.startup) {
        MathJax.startup.promise.then(() => {
          MathJax.typesetPromise();
        });
      }
    }
  });
}