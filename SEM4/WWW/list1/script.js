
document.addEventListener('DOMContentLoaded', () => {
    const btnToggle = document.getElementById('btn-toggle');
    const mainNav = document.getElementById('main-nav');
  
    btnToggle.addEventListener('click', () => {
      mainNav.classList.toggle('active');
    });
  });
  