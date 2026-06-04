const PageSummary = {
  render() {
    const container = document.getElementById('page-summary');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Review your settings</h1>
      </div>
      <p class="content-lead">We'll apply these settings to your device.</p>
      <div class="scroll-view" style="max-height:300px">
        <div class="list">
          <div class="list-item non-interactive">
            <span class="list-icon icon icon-globe"></span>
            <div class="container-text">
              <div class="containertext-item">
                <div class="containertext-title">Region</div>
                <div class="secondary-container-text" id="summary-region">-</div>
              </div>
            </div>
          </div>
          <div class="list-item non-interactive">
            <span class="list-icon icon icon-win-keyboardStandard"></span>
            <div class="container-text">
              <div class="containertext-item">
                <div class="containertext-title">Keyboard layout</div>
                <div class="secondary-container-text" id="summary-keyboard">-</div>
              </div>
            </div>
          </div>
          <div class="list-item non-interactive">
            <span class="list-icon icon icon-connect-small"></span>
            <div class="container-text">
              <div class="containertext-item">
                <div class="containertext-title">Network</div>
                <div class="secondary-container-text" id="summary-network">-</div>
              </div>
            </div>
          </div>
          <div class="list-item non-interactive">
            <span class="list-icon icon icon-user"></span>
            <div class="container-text">
              <div class="containertext-item">
                <div class="containertext-title">Account name</div>
                <div class="secondary-container-text" id="summary-account">-</div>
              </div>
            </div>
          </div>
          <div class="list-item non-interactive">
            <span class="list-icon icon icon-smile"></span>
            <div class="container-text">
              <div class="containertext-item">
                <div class="containertext-title">Device name</div>
                <div class="secondary-container-text" id="summary-device">-</div>
              </div>
            </div>
          </div>
        </div>
      </div>
    `;

    document.getElementById('summary-region').textContent = AppState.get('region') || '-';
    document.getElementById('summary-keyboard').textContent = AppState.get('keyboard') || '-';
    document.getElementById('summary-network').textContent = AppState.get('network') || '-';
    document.getElementById('summary-account').textContent = AppState.get('accountName') || '-';
    document.getElementById('summary-device').textContent = AppState.get('deviceName') || '-';
  }
};
