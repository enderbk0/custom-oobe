const KEYBOARD_LAYOUTS = [
  { code: 'en-US', name: 'US' },
  { code: 'en-GB', name: 'United Kingdom' },
  { code: 'de-DE', name: 'German' },
  { code: 'fr-FR', name: 'French' },
  { code: 'es-ES', name: 'Spanish' },
  { code: 'it-IT', name: 'Italian' },
  { code: 'pt-BR', name: 'Portuguese (Brazil)' },
  { code: 'ja-JP', name: 'Japanese' },
  { code: 'ko-KR', name: 'Korean' },
  { code: 'zh-CN', name: 'Chinese (Simplified)' },
  { code: 'ru-RU', name: 'Russian' },
  { code: 'ar-SA', name: 'Arabic (Saudi Arabia)' },
  { code: 'nl-NL', name: 'Dutch' },
  { code: 'sv-SE', name: 'Swedish' },
  { code: 'da-DK', name: 'Danish' },
  { code: 'nb-NO', name: 'Norwegian' },
  { code: 'fi-FI', name: 'Finnish' },
  { code: 'pl-PL', name: 'Polish' },
  { code: 'tr-TR', name: 'Turkish' },
  { code: 'he-IL', name: 'Hebrew' },
];

const PageKeyboard = {
  render() {
    const container = document.getElementById('page-keyboard');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Is this the right keyboard layout?</h1>
      </div>
      <p class="content-lead">Choose the keyboard layout that matches your keyboard.</p>
      <fieldset>
        <legend>Keyboard layout</legend>
        <div class="scroll-view" style="max-height:300px">
          <div class="list" id="keyboard-list">
            ${KEYBOARD_LAYOUTS.map(k => `
              <div class="list-item" data-layout="${k.code}" tabindex="0" role="button">
                <span class="list-item-title">${k.name}</span>
              </div>
            `).join('')}
          </div>
        </div>
      </fieldset>
    `;

    const items = container.querySelectorAll('.list-item');
    items.forEach(item => {
      item.addEventListener('click', () => {
        items.forEach(i => i.classList.remove('selected'));
        item.classList.add('selected');
        AppState.set('keyboard', item.dataset.layout);
        Bridge.setKeyboardLayout(item.dataset.layout);
      });
      item.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          item.click();
        }
      });
    });

    const saved = AppState.get('keyboard');
    if (saved) {
      const selected = container.querySelector(`[data-layout="${saved}"]`);
      if (selected) selected.classList.add('selected');
    }
  },

  onBeforeNext() {
    const sel = AppState.get('keyboard');
    if (!sel) return false;
    return true;
  }
};
