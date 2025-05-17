/*jslint browser: true, es6: true */
document.addEventListener('DOMContentLoaded', () => {
  const imageSrcs = [
      './agwiazdkazdjecie.jpg',
      './ai.png',
      './chinesecheckers.jpeg',
      './hoi.jpg',
      './sejm.png',
      './tuskkaczo22r.jpeg'
  ];

  function loadImage(src) {
      return new Promise((resolve, reject) => {
          const img = new Image();
          img.src = src;
          img.alt = `Obrazek: ${src}`;       
          img.onload  = () => resolve(img);
          img.onerror = () => reject(src);   
      });
  }

  Promise.allSettled(imageSrcs.map(loadImage))
      .then(results => {
          const gallery = document.getElementById('galeria');

          results.forEach((result, i) => {
              const wrapper = document.createElement('div');
              wrapper.className = 'gallery-item';

              if (result.status === 'fulfilled') {
                  wrapper.appendChild(result.value);
              } else {
                  const placeholder = document.createElement('span');
                  placeholder.className = 'image-alt';
                  placeholder.textContent = `Nie można wczytać: ${result.reason}`;
                  wrapper.appendChild(placeholder);
              }

              gallery.appendChild(wrapper);
          });
      })
      .catch(err => {
          console.error('Coś poszło nie tak przy ładowaniu galerii:', err);
      });
});
