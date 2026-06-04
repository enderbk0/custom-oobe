const KEYBOARD_LAYOUTS = [
  { code: 'en-US', name: 'US' },
  { code: 'en-GB', name: 'United Kingdom' },
  { code: 'fr-FR', name: 'French (France)' },
  { code: 'de-DE', name: 'German (Germany)' },
  { code: 'es-ES', name: 'Spanish (Spain)' },
  { code: 'it-IT', name: 'Italian (Italy)' },
  { code: 'pt-BR', name: 'Portuguese (Brazil)' },
  { code: 'nl-NL', name: 'Dutch (Netherlands)' },
  { code: 'sv-SE', name: 'Swedish (Sweden)' },
  { code: 'nb-NO', name: 'Norwegian (Bokmal)' },
  { code: 'da-DK', name: 'Danish (Denmark)' },
  { code: 'fi-FI', name: 'Finnish (Finland)' },
  { code: 'pl-PL', name: 'Polish (Poland)' },
  { code: 'ru-RU', name: 'Russian (Russia)' },
  { code: 'ja-JP', name: 'Japanese (Japan)' },
  { code: 'ko-KR', name: 'Korean (Korea)' },
  { code: 'zh-CN', name: 'Chinese (Simplified)' },
  { code: 'zh-TW', name: 'Chinese (Traditional)' },
  { code: 'ar-SA', name: 'Arabic (Saudi Arabia)' },
  { code: 'tr-TR', name: 'Turkish (Turkey)' },
  { code: 'cs-CZ', name: 'Czech (Czech Republic)' },
  { code: 'hu-HU', name: 'Hungarian (Hungary)' },
  { code: 'ro-RO', name: 'Romanian (Romania)' },
  { code: 'th-TH', name: 'Thai (Thailand)' },
  { code: 'vi-VN', name: 'Vietnamese (Vietnam)' },
  { code: 'he-IL', name: 'Hebrew (Israel)' },
];

const PageKeyboard = {
  render() {
    const container = document.getElementById('page-keyboard');
    container.innerHTML = `
      <h1>What's your keyboard layout?</h1>
      <p class="subtitle">Choose the layout that matches your keyboard.</p>
      <div class="content-area">
        <div class="keyboard-layout-list options-list">
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
        AppState.set('keyboardLayout', card.dataset.layout);
        Bridge.setKeyboardLayout(card.dataset.layout);
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

    const saved = AppState.get('keyboardLayout');
    if (saved) {
      const selected = container.querySelector(`[data-layout="${saved}"]`);
      if (selected) selected.classList.add('selected');
    }
  },

  onBeforeNext() {
    const selected = AppState.get('keyboardLayout');
    if (!selected) {
      return false;
    }
    return true;
  }
};
