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
      <h1>Is this the right region?</h1>
      <p class="subtitle">We use your region to give you the right experience.</p>
      <div class="content-area">
        <div class="region-list options-list">
          ${REGIONS.map(r => `
            <div class="selection-card" data-region="${r.code}" tabindex="0" role="button">
              <div class="selection-card-content">
                <div class="selection-card-title">${r.name}</div>
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
        AppState.set('region', card.dataset.region);
        Bridge.setRegion(card.dataset.region);
      });
      card.addEventListener('keydown', (e) => {
        if (e.key === 'Enter' || e.key === ' ') {
          e.preventDefault();
          card.click();
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
    if (!selected) {
      const container = document.getElementById('page-region');
      let errorEl = container.querySelector('.form-error');
      if (!errorEl) {
        errorEl = document.createElement('div');
        errorEl.className = 'form-error';
        errorEl.style.textAlign = 'center';
        container.querySelector('.content-area').after(errorEl);
      }
      errorEl.textContent = 'Please select a region before continuing';
      return false;
    }
    return true;
  }
};
