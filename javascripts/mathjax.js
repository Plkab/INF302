// ============================================================
// CONFIGURATION MATHJAX v3 – Version définitive
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
    processEscapes: true,
    processEnvironments: true
  },
  svg: {
    fontCache: 'global'
  },
  options: {
    // On analyse tout le document sans restriction de classe
    ignoreHtmlClass: '',
    processHtmlClass: ''
  }
};

// ============================================================
// FONCTION DE RENDU (avec gestion d'erreur)
// ============================================================

function renderMath() {
  if (window.MathJax && MathJax.typesetPromise) {
    MathJax.typesetPromise().catch(function(err) {
      console.warn('MathJax: erreur de rendu –', err);
    });
  }
}

// ============================================================
// DÉCLENCHEMENT AU CHARGEMENT INITIAL
// ============================================================

if (document.readyState === 'complete') {
  renderMath();
} else {
  document.addEventListener('readystatechange', function() {
    if (document.readyState === 'complete') {
      renderMath();
    }
  });
}

// ============================================================
// OBSERVATEUR DE MUTATIONS – pour les notebooks chargés dynamiquement
// ============================================================

const observer = new MutationObserver(function(mutations) {
  for (let mutation of mutations) {
    if (mutation.addedNodes.length > 0) {
      renderMath();
      break;
    }
  }
});

// Démarrer l'observation une fois le DOM prêt
if (document.body) {
  observer.observe(document.body, {
    childList: true,
    subtree: true
  });
} else {
  document.addEventListener('DOMContentLoaded', function() {
    observer.observe(document.body, {
      childList: true,
      subtree: true
    });
  });
}