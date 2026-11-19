const practicals = [
  {
    "number": "01",
    "title": "Number system conversions",
    "language": "C",
    "file": "practical_01_conversions.c",
    "topic": "Binary, decimal and hexadecimal conversions",
    "group": "C Programs"
  },
  {
    "number": "02",
    "title": "1’s and 2’s complement",
    "language": "C",
    "file": "practical_02_complements.c",
    "topic": "8-bit complement operations",
    "group": "C Programs"
  },
  {
    "number": "03",
    "title": "Circular left and right shift",
    "language": "C",
    "file": "practical_03_circular_shift.c",
    "topic": "8-bit rotate operations",
    "group": "C Programs"
  },
  {
    "number": "05",
    "title": "Addition and subtraction of 8-bit numbers",
    "language": "8085 Assembly",
    "file": "practical_05_add_sub.asm",
    "topic": "GNUSim8085",
    "group": "8085 Assembly"
  },
  {
    "number": "06",
    "title": "1’s and 2’s complement",
    "language": "8085 Assembly",
    "file": "practical_06_complements.asm",
    "topic": "GNUSim8085",
    "group": "8085 Assembly"
  },
  {
    "number": "07",
    "title": "Sum of five numbers using a loop",
    "language": "8085 Assembly",
    "file": "practical_07_sum_of_five.asm",
    "topic": "GNUSim8085",
    "group": "8085 Assembly"
  },
  {
    "number": "08",
    "title": "Find the smallest number in an array",
    "language": "8085 Assembly",
    "file": "practical_08_smallest_in_array.asm",
    "topic": "GNUSim8085",
    "group": "8085 Assembly"
  },
  {
    "number": "09",
    "title": "Matrix multiplication with CUDA",
    "language": "CUDA C++",
    "file": "practical_09_cuda_matmul.cu",
    "topic": "GPU matrix multiplication",
    "group": "CUDA"
  },
  {
    "number": "10",
    "title": "CUDA global memory read and write",
    "language": "CUDA C++",
    "file": "practical_10_cuda_memory_rw.cu",
    "topic": "GPU memory access",
    "group": "CUDA"
  },
  {
    "number": "11",
    "title": "Large-array arithmetic with OpenCL",
    "language": "OpenCL C",
    "file": "practical_11_opencl_large_arrays.c",
    "topic": "Addition, subtraction and multiplication",
    "group": "OpenCL"
  },
  {
    "number": "12",
    "title": "OpenCL work-group communication and reduction",
    "language": "OpenCL C",
    "file": "practical_12_opencl_communication.c",
    "topic": "Local memory and barriers",
    "group": "OpenCL"
  }
];
const grid = document.querySelector('#practical-grid');
const search = document.querySelector('#search');
const filterButtons = [...document.querySelectorAll('[data-filter]')];
let activeFilter = 'All';

function escapeText(value) {
  return String(value).replace(/[&<>"']/g, ch => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[ch]));
}

function makeCard(item) {
  const card = document.createElement('article');
  card.className = 'practical-card';
  card.dataset.group = item.group;
  card.dataset.search = `${item.number} ${item.title} ${item.language} ${item.file} ${item.topic}`.toLowerCase();
  card.innerHTML = `
    <div class="card-topline">
      <span class="practical-number">P${escapeText(item.number)}</span>
      <span class="language-tag">${escapeText(item.language)}</span>
    </div>
    <h3>${escapeText(item.title)}</h3>
    <p class="topic">${escapeText(item.topic)}</p>
    <div class="card-footer">
      <span class="filename">${escapeText(item.file)}</span>
      <button class="view-button" type="button" aria-expanded="false">View code <span aria-hidden="true">↗</span></button>
    </div>
    <section class="code-panel" hidden>
      <div class="code-actions">
        <span>Source code</span>
        <div>
          <button class="small-button copy-button" type="button">Copy</button>
          <a class="small-button download-button" href="practicals/${encodeURIComponent(item.file)}" download>Download</a>
        </div>
      </div>
      <pre><code>Loading source…</code></pre>
      <p class="load-error" hidden>Could not load this file. Use Download to access the source directly.</p>
    </section>`;
  const viewButton = card.querySelector('.view-button');
  const panel = card.querySelector('.code-panel');
  const code = card.querySelector('pre code');
  let loaded = false;
  let codePromise = null;
  async function loadCode() {
    if (loaded) return;
    if (!codePromise) {
      codePromise = (async () => {
        const response = await fetch(`practicals/${encodeURIComponent(item.file)}`);
        if (!response.ok) throw new Error(`HTTP ${response.status}`);
        code.textContent = await response.text();
        loaded = true;
      })();
    }
    return codePromise;
  }
  viewButton.addEventListener('click', async () => {
    const opening = panel.hidden;
    panel.hidden = !opening;
    viewButton.setAttribute('aria-expanded', String(opening));
    viewButton.innerHTML = opening ? 'Hide code <span aria-hidden="true">↙</span>' : 'View code <span aria-hidden="true">↗</span>';
    if (opening && !loaded) {
      try {
        await loadCode();
      } catch (error) {
        code.textContent = '';
        card.querySelector('.load-error').hidden = false;
      }
    }
  });
  card.querySelector('.copy-button').addEventListener('click', async (event) => {
    const copyButton = event.currentTarget;
    if (panel.hidden) {
      panel.hidden = false;
      viewButton.setAttribute('aria-expanded', 'true');
      viewButton.innerHTML = 'Hide code <span aria-hidden="true">↙</span>';
    }
    try {
      await loadCode();
      await navigator.clipboard.writeText(code.textContent);
      copyButton.textContent = 'Copied';
      setTimeout(() => copyButton.textContent = 'Copy', 1400);
    } catch {
      card.querySelector('.load-error').hidden = false;
      copyButton.textContent = 'Copy failed';
      setTimeout(() => copyButton.textContent = 'Copy', 1400);
    }
  });
  return card;
}

const cards = practicals.map(makeCard);
cards.forEach(card => grid.append(card));

function applyFilters() {
  const query = search.value.trim().toLowerCase();
  let shown = 0;
  cards.forEach(card => {
    const groupMatch = activeFilter === 'All' || card.dataset.group === activeFilter;
    const queryMatch = card.dataset.search.includes(query);
    card.hidden = !(groupMatch && queryMatch);
    if (!card.hidden) shown++;
  });
  document.querySelector('#empty-state').hidden = shown !== 0;
  document.querySelector('#result-count').textContent = `${shown} of ${cards.length} practicals`;
}

search.addEventListener('input', applyFilters);
filterButtons.forEach(button => button.addEventListener('click', () => {
  activeFilter = button.dataset.filter;
  filterButtons.forEach(other => {
    const selected = other === button;
    other.classList.toggle('active', selected);
    other.setAttribute('aria-pressed', String(selected));
  });
  applyFilters();
}));
applyFilters();
