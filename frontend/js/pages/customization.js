const PageCustomization = {
  render() {
    const container = document.getElementById('page-customization');
    container.innerHTML = `
      <h1>Customize your experience</h1>
      <p class="subtitle">Tell us how you plan to use this device so we can tailor your experience.</p>
      <div class="content-area">
        <div class="selection-card" data-value="entertainment" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Entertainment</div>
            <div class="selection-card-subtitle">Watch movies, play games, and stream content</div>
          </div>
        </div>
        <div class="mt-sm selection-card" data-value="gaming" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Gaming</div>
            <div class="selection-card-subtitle">Play PC games</div>
          </div>
        </div>
        <div class="mt-sm selection-card" data-value="school" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Schoolwork</div>
            <div class="selection-card-subtitle">Take notes, write papers, and do research</div>
          </div>
        </div>
        <div class="mt-sm selection-card" data-value="creativity" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Creativity</div>
            <div class="selection-card-subtitle">Design, draw, edit photos, and make music</div>
          </div>
        </div>
        <div class="mt-sm selection-card" data-value="business" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Business</div>
            <div class="selection-card-subtitle">Work, email, and productivity</div>
          </div>
        </div>
        <div class="mt-sm selection-card" data-value="family" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Family</div>
            <div class="selection-card-subtitle">Stay connected with family</div>
          </div>
        </div>
      </div>
    `;

    const cards = container.querySelectorAll('.selection-card');
    cards.forEach(card => {
      card.addEventListener('click', () => {
        cards.forEach(c => c.classList.remove('selected'));
        card.classList.add('selected');
        AppState.set('customization', card.dataset.value);
        Bridge.setCustomization({ usage: card.dataset.value });
        setTimeout(() => {
          document.getElementById('btn-next').focus();
        }, 100);
      });
      card.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          card.click();
        }
      });
    });
  },

  onBeforeNext() {
    return true;
  }
};
