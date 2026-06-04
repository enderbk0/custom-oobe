const NETWORKS = [
  { ssid: 'HomeNetwork', secured: true, strength: 3 },
  { ssid: 'WorkWiFi', secured: true, strength: 4 },
  { ssid: 'Guest_5GHz', secured: true, strength: 2 },
  { ssid: 'CoffeeShop', secured: false, strength: 1 },
  { ssid: 'Library_Public', secured: false, strength: 3 },
  { ssid: 'School_Campus', secured: true, strength: 2 },
  { ssid: 'Neighbor_2G', secured: true, strength: 1 },
  { ssid: 'Office_Ext', secured: true, strength: 3 },
];

const WIFI_ICONS = ['', '\uE701', '\uE701', '\uE701', '\uE701'];

const PageNetwork = {
  render() {
    const container = document.getElementById('page-network');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Let's connect you to a network</h1>
      </div>
      <p class="content-lead">Choose a network to connect to during setup.</p>
      <fieldset>
        <legend>Select a network</legend>
        <div class="scroll-view" style="max-height:280px">
          <div class="list" id="network-list">
            ${NETWORKS.map((n, i) => `
              <div class="list-item ${i === 0 ? 'selected' : ''}" data-ssid="${n.ssid}" tabindex="0" role="button">
                <span class="list-icon icon icon-connect-small"></span>
                <div class="container-text">
                  <div class="containertext-item">
                    <div class="containertext-title">${n.ssid}</div>
                    <div class="secondary-container-text">${n.secured ? 'Secured' : 'Open'}</div>
                  </div>
                </div>
              </div>
            `).join('')}
          </div>
        </div>
      </fieldset>
      <div id="network-password-area" style="display:none">
        <fieldset>
          <legend>Network password</legend>
          <div class="template-input">
            <input type="password" id="network-password" class="win-textbox" placeholder="Enter network password" />
          </div>
        </fieldset>
      </div>
    `;

    const items = container.querySelectorAll('.list-item');
    items.forEach(item => {
      item.addEventListener('click', () => {
        items.forEach(i => i.classList.remove('selected'));
        item.classList.add('selected');
        const ssid = item.dataset.ssid;
        AppState.set('network', ssid);
        Bridge.setNetwork(ssid);
        const network = NETWORKS.find(n => n.ssid === ssid);
        const pwArea = document.getElementById('network-password-area');
        if (pwArea) pwArea.style.display = network && network.secured ? 'block' : 'none';
      });
    });
  },

  onBeforeNext() {
    const sel = AppState.get('network');
    if (!sel) return false;
    return true;
  }
};
