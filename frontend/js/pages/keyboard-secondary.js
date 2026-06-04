const PageKeyboardSecondary = {
  render() {
    const container = document.getElementById('page-keyboard-secondary');
    container.innerHTML = `
      <h1>Add a second keyboard layout?</h1>
      <p class="subtitle">You can always add another layout later in Settings.</p>
      <div class="content-area">
        <div class="selection-card" data-action="skip" tabindex="0" role="button">
          <div class="selection-card-content">
            <div class="selection-card-title">Skip</div>
            <div class="selection-card-subtitle">I'll add one later</div>
          </div>
        </div>
        <div class="mt-md keyboard-layout-list options-list">
          ${KEYBOARD_LAYOUTS.map(k => `
            <div class="selection-card" data-layout="${k.code}" tabindex="0" role="button">
              <div class="selection-card-content">
                <div class="selection-card-title">${k.name}</div>
              </div>
            </div>
          `).join('')}
        </div>
      </div>
    `;

    const cards = container.querySelectorAll('.selection-card');
    cards.forEach(card => {
      card.addEventListener('click', () => {
        cards.forEach(c => c.classList.remove('selected'));
        card.classList.add('selected');
        if (card.dataset.layout) {
          AppState.set('keyboardLayoutSecondary', card.dataset.layout);
          Bridge.setKeyboardLayoutSecondary(card.dataset.layout);
        } else {
          AppState.set('keyboardLayoutSecondary', '');
        }
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
  }
};
