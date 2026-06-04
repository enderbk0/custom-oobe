const PageNetwork = {
  render() {
    const container = document.getElementById('page-network');
    container.innerHTML = `
      <h1>Let's connect you to a network</h1>
      <p class="subtitle">Choose a network to connect to, or connect later.</p>
      <div class="content-area">
        <div class="network-list">
          <div class="selection-card" data-action="skip" tabindex="0" role="button">
            <div class="selection-card-icon network-icon">
              <svg width="24" height="24" viewBox="0 0 24 24" fill="none">
                <path d="M12 22C13.1046 22 14 21.1046 14 20C14 18.8954 13.1046 18 12 18C10.8954 18 10 18.8954 10 20C10 21.1046 10.8954 22 12 22Z" fill="currentColor"/>
                <path d="M12 14C14.2091 14 16 15.7909 16 18H8C8 15.7909 9.79086 14 12 14Z" fill="currentColor" opacity="0.6"/>
                <path d="M12 10C15.3137 10 18 12.6863 18 16H6C6 12.6863 8.68629 10 12 10Z" fill="currentColor" opacity="0.4"/>
              </svg>
            </div>
            <div class="selection-card-content">
              <div class="selection-card-title">I don't have internet</div>
              <div class="selection-card-subtitle">Continue with limited setup</div>
            </div>
          </div>
          <div class="mt-md">
            <div class="radio-group" id="network-list">
              <div class="radio-option">
                <input type="radio" name="network" id="net-eth" value="ethernet">
                <label for="net-eth" class="radio-label">Ethernet (Connected)</label>
              </div>
              <div class="radio-option">
                <input type="radio" name="network" id="net-wifi" value="wifi" disabled>
                <label for="net-wifi" class="radio-label">Wi-Fi (No networks found)</label>
              </div>
            </div>
          </div>
        </div>
      </div>
    `;

    container.querySelector('[data-action="skip"]').addEventListener('click', () => {
      AppState.set('networkConnected', false);
      Bridge.setNetworkConfig({ connected: false });
      Router.goForward();
    });
  }
};
