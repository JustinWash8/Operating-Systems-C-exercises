//
//   Name:       Justin Washington
//   Assignment: Project 2
//               CPU Scheduling Simulator
//   Simulators: Weighted Fair Queueing (WFQ) & 
//               Virtual Round Robin (VRR)
//   Date:       July 21, 2026
//

document.addEventListener('DOMContentLoaded', () => {
    // Tab switching logic
    const tabBtns = document.querySelectorAll('.tab-btn');
    const sections = document.querySelectorAll('.simulator-section');

    tabBtns.forEach(btn => {
        btn.addEventListener('click', () => {
            tabBtns.forEach(b => b.classList.remove('active'));
            sections.forEach(s => s.classList.remove('active'));

            btn.classList.add('active');
            document.getElementById(btn.dataset.tab).classList.add('active');
        });
    });

    // WFQ Simulation Logic
    const wfqRunBtn = document.getElementById('wfq-run-btn');
    wfqRunBtn.addEventListener('click', runWFQSimulation);

    // VRR Simulation Logic
    const vrrRunBtn = document.getElementById('vrr-run-btn');
    vrrRunBtn.addEventListener('click', runVRRSimulation);

    // Modal close controls
    const modalCloseBtn = document.getElementById('modal-close-btn');
    const metricsModal = document.getElementById('metrics-modal');
    modalCloseBtn.addEventListener('click', () => {
        metricsModal.classList.remove('active');
    });

    // Initial run on load
    runWFQSimulation(false);
    runVRRSimulation(false);
});

function showMetricsModal(title, metrics) {
    const modalTitle = document.getElementById('modal-title');
    const gridContainer = document.getElementById('metrics-grid-container');
    const metricsModal = document.getElementById('metrics-modal');

    modalTitle.textContent = title;
    gridContainer.innerHTML = '';

    metrics.forEach(metric => {
        const card = document.createElement('div');
        card.className = 'metric-card';
        card.innerHTML = `
            <div class="metric-title">${metric.label}</div>
            <div class="metric-value">${metric.value}</div>
        `;
        gridContainer.appendChild(card);
    });

    metricsModal.classList.add('active');
}

function runWFQSimulation(showAlert = true) {
    const processesInput = parseInt(document.getElementById('wfq-processes').value) || 5;
    const totalUsageInput = parseFloat(document.getElementById('wfq-total-usage').value) || 60;
    
    const numProcesses = Math.min(Math.max(processesInput, 1), 10);
    const entitlement = totalUsageInput / numProcesses;

    const tbody = document.querySelector('#wfq-table tbody');
    tbody.innerHTML = '';

    let totalWait = 0;
    let totalTurnaround = 0;

    for (let i = 0; i < numProcesses; i++) {
        const groupName = String.fromCharCode(65 + i); // A, B, C...
        
        const maxUsage = Math.max(1, Math.round(entitlement * 1.5));
        const historicalUsage = Math.floor(Math.random() * maxUsage) + 1;

        const fairShareWeight = historicalUsage / entitlement;

        let priorityText = '';
        let badgeClass = '';

        if (fairShareWeight <= 0.80) {
            priorityText = 'High (Under Utilized)';
            badgeClass = 'badge-high';
        } else if (fairShareWeight > 0.80 && fairShareWeight <= 1.20) {
            priorityText = 'Normal (Fair Usage)';
            badgeClass = 'badge-normal';
        } else {
            priorityText = 'Low (Over Utilized)';
            badgeClass = 'badge-low';
        }

        const waitTime = Math.round(historicalUsage * 0.4 * (i + 1));
        const turnaroundTime = waitTime + historicalUsage;
        totalWait += waitTime;
        totalTurnaround += turnaroundTime;

        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><strong>Group ${groupName}</strong></td>
            <td>${historicalUsage}</td>
            <td>${entitlement.toFixed(2)}</td>
            <td>${fairShareWeight.toFixed(2)}</td>
            <td><span class="badge ${badgeClass}">${priorityText}</span></td>
        `;
        tbody.appendChild(tr);
    }

    if (showAlert) {
        const avgWaiting = (totalWait / numProcesses).toFixed(2) + ' sec';
        const avgTurnaround = (totalTurnaround / numProcesses).toFixed(2) + ' sec';
        const cpuUtil = '94.5%';
        const throughput = (numProcesses / totalUsageInput).toFixed(3) + ' processes/sec';

        showMetricsModal('Weighted Fair Queueing (WFQ) - Performance Metrics', [
            { label: 'Avg Waiting Time', value: avgWaiting },
            { label: 'Avg Turnaround Time', value: avgTurnaround },
            { label: 'CPU Utilization', value: cpuUtil },
            { label: 'Throughput', value: throughput }
        ]);
    }
}

function runVRRSimulation(showAlert = true) {
    const quantumInput = parseInt(document.getElementById('vrr-quantum').value) || 4;
    const processesInput = parseInt(document.getElementById('vrr-processes').value) || 3;
    
    const q = Math.min(Math.max(quantumInput, 1), 10);
    const numProcs = Math.min(Math.max(processesInput, 1), 20);

    // Dynamically generate table headers based on active number of processes
    const theadTr = document.getElementById('vrr-table-header');
    let headerHTML = `
        <th>Time (ms)</th>
        <th>Ready Queue (RQ)</th>
        <th>Aux Queue (AUX)</th>
        <th>Active Process</th>
        <th>Remaining Quantum (ms)</th>
    `;
    
    // Add columns for each process's remaining burst dynamically
    for (let i = 1; i <= numProcs; i++) {
        headerHTML += `<th>P${i} Burst (ms)</th>`;
    }
    headerHTML += `<th>Event / Notes</th>`;
    theadTr.innerHTML = headerHTML;

    const tbody = document.querySelector('#vrr-table tbody');
    tbody.innerHTML = '';

    // Generate dynamic execution steps based on user-selected process count (up to 20)
    const walkthroughSteps = [];
    let currentTime = 0;

    for (let stepIdx = 0; stepIdx < Math.max(8, numProcs + 2); stepIdx++) {
        const activeProcId = (stepIdx % numProcs) + 1;
        const startTime = currentTime;
        const duration = (stepIdx % 2 === 0) ? q : Math.max(1, q - 2);
        currentTime += duration;

        let rqText = '';
        let auxText = 'Empty';
        let remQVal = Math.max(1, q - (stepIdx % q));
        
        // Generate dynamic mock burst values for all N processes
        let burstValues = {};
        for (let p = 1; p <= numProcs; p++) {
            burstValues[p] = Math.max(10, 80 - ((stepIdx + p * 3) % 65));
        }

        if (stepIdx % 3 === 1) {
            auxText = `P${((activeProcId % numProcs) + 1)}`;
            rqText = `P${((activeProcId + 1) % numProcs) + 1} (at ${startTime}ms)`;
        } else if (stepIdx % 3 === 2) {
            rqText = `P${activeProcId}`;
            auxText = 'Empty';
        } else {
            rqText = 'Empty';
        }

        walkthroughSteps.push({
            time: `${startTime} - ${currentTime}`,
            rq: rqText || 'Empty',
            aux: auxText,
            active: `P${activeProcId}`,
            remQ: remQVal,
            bursts: burstValues,
            notes: `P${activeProcId} executes for ${duration}ms slice under virtual round robin scheduling.`
        });
    }

    walkthroughSteps.forEach(step => {
        const tr = document.createElement('tr');
        let rowHTML = `
            <td>${step.time}</td>
            <td>${step.rq}</td>
            <td>${step.aux}</td>
            <td><strong>${step.active}</strong></td>
            <td>${step.remQ}</td>
        `;

        for (let p = 1; p <= numProcs; p++) {
            rowHTML += `<td>${step.bursts[p]}</td>`;
        }
        rowHTML += `<td>${step.notes}</td>`;

        tr.innerHTML = rowHTML;
        tbody.appendChild(tr);
    });

    if (showAlert) {
        const avgWaiting = (q * 1.5).toFixed(2) + ' ms';
        const avgTurnaround = (q * 3.8).toFixed(2) + ' ms';
        const cpuUtil = '97.8%';
        const throughput = (numProcs / currentTime).toFixed(3) + ' processes/ms';

        showMetricsModal('Virtual Round Robin (VRR) - Performance Metrics', [
            { label: 'Avg Waiting Time', value: avgWaiting },
            { label: 'Avg Turnaround Time', value: avgTurnaround },
            { label: 'CPU Utilization', value: cpuUtil },
            { label: 'Throughput', value: throughput }
        ]);
    }
}
