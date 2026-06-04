const REGIONS = [
  { code: 'US', name: 'United States' },
  { code: 'CA', name: 'Canada' },
  { code: 'GB', name: 'United Kingdom' },
  { code: 'AU', name: 'Australia' },
  { code: 'DE', name: 'Germany' },
  { code: 'FR', name: 'France' },
  { code: 'JP', name: 'Japan' },
  { code: 'CN', name: 'China' },
  { code: 'IN', name: 'India' },
  { code: 'BR', name: 'Brazil' },
  { code: 'MX', name: 'Mexico' },
  { code: 'IT', name: 'Italy' },
  { code: 'ES', name: 'Spain' },
  { code: 'NL', name: 'Netherlands' },
  { code: 'SE', name: 'Sweden' },
  { code: 'NO', name: 'Norway' },
  { code: 'DK', name: 'Denmark' },
  { code: 'FI', name: 'Finland' },
  { code: 'PL', name: 'Poland' },
  { code: 'RU', name: 'Russia' },
  { code: 'KR', name: 'South Korea' },
  { code: 'SG', name: 'Singapore' },
  { code: 'NZ', name: 'New Zealand' },
  { code: 'ZA', name: 'South Africa' },
  { code: 'AE', name: 'United Arab Emirates' },
  { code: 'SA', name: 'Saudi Arabia' },
  { code: 'IL', name: 'Israel' },
  { code: 'TR', name: 'Turkey' },
  { code: 'CH', name: 'Switzerland' },
  { code: 'AT', name: 'Austria' },
  { code: 'BE', name: 'Belgium' },
  { code: 'PT', name: 'Portugal' },
  { code: 'IE', name: 'Ireland' },
  { code: 'HK', name: 'Hong Kong SAR' },
  { code: 'TW', name: 'Taiwan' },
];

const PageRegion = {
  render() {
    const container = document.getElementById('page-region');
    container.innerHTML = `
      <div class="body-header">
        <h1 class="text-title">Is this the right region?</h1>
      </div>
      <p class="content-lead">We use your region to give you the right experience.</p>
      <fieldset>
        <legend>Select your region</legend>
        <div class="scroll-view" style="max-height:300px">
          <div class="list" id="region-list">
            ${REGIONS.map(r => `
              <div class="list-item" data-region="${r.code}" tabindex="0" role="button">
                <span class="list-item-title">${r.name}</span>
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
        AppState.set('region', item.dataset.region);
        Bridge.setRegion(item.dataset.region);
      });
      item.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          item.click();
        }
      });
    });

    const saved = AppState.get('region');
    if (saved) {
      const selected = container.querySelector(`[data-region="${saved}"]`);
      if (selected) selected.classList.add('selected');
    }
  },

  onBeforeNext() {
    const selected = AppState.get('region');
    if (!selected) return false;
    return true;
  }
};
