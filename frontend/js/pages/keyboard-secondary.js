const PageKeyboardSecondary = {
  render() {
    const container = document.getElementById('page-keyboard-secondary');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Add a second keyboard layout?</h1>
      </div>
      <p class="content-lead">You can use a second keyboard layout if your keyboard supports multiple layouts.</p>
      <fieldset>
        <legend>Second keyboard layout</legend>
        <div class="template-input">
          <select class="win-dropdown" id="keyboard-secondary-select">
            <option value="">None</option>
            <option value="en-US">US</option>
            <option value="en-GB">United Kingdom</option>
            <option value="de-DE">German</option>
            <option value="fr-FR">French</option>
            <option value="es-ES">Spanish</option>
            <option value="it-IT">Italian</option>
            <option value="pt-BR">Portuguese (Brazil)</option>
            <option value="ja-JP">Japanese</option>
            <option value="ko-KR">Korean</option>
            <option value="zh-CN">Chinese (Simplified)</option>
            <option value="ru-RU">Russian</option>
          </select>
        </div>
      </fieldset>
    `;

    const sel = document.getElementById('keyboard-secondary-select');
    sel.addEventListener('change', () => {
      AppState.set('keyboardSecondary', sel.value || null);
      Bridge.setKeyboardSecondaryLayout(sel.value || null);
    });

    const saved = AppState.get('keyboardSecondary');
    if (saved) sel.value = saved;
  }
};
