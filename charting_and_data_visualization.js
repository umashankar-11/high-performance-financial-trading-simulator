<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Real-Time Financial Chart</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
  <style>
    body {
      font-family: Arial, sans-serif;
    }
    #chart-container {
      position: relative;
      margin: auto;
      width: 80%;
      height: 600px;
    }
    .info {
      text-align: center;
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <div id="chart-container">
    <canvas id="myChart"></canvas>
  </div>
  <div class="info">
    <button id="startSimulation">Start Simulation</button>
    <p id="status">Simulation Not Started</p>
  </div>

  <script>
    let chartData = {
      labels: [],
      datasets: [{
        label: 'Price',
        data: [],
        borderColor: 'rgba(0, 123, 255, 1)',
        backgroundColor: 'rgba(0, 123, 255, 0.2)',
        borderWidth: 1,
        fill: true,
        lineTension: 0.1,
      }]
    };

    let chartConfig = {
      type: 'line',
      data: chartData,
      options: {
        responsive: true,
        scales: {
          x: {
            type: 'linear',
            position: 'bottom',
            ticks: {
              autoSkip: true,
              maxTicksLimit: 10,
            }
          },
          y: {
            beginAtZero: false,
            ticks: {
              autoSkip: true,
              maxTicksLimit: 5
            }
          }
        },
        animation: {
          duration: 0
        }
      }
    };

    let ctx = document.getElementById('myChart').getContext('2d');
    let myChart = new Chart(ctx, chartConfig);

    let simulationInterval;
    let xValue = 0;
    let priceValue = 100;
    let isSimulating = false;

    function simulateData() {
      if (isSimulating) {
        let priceChange = (Math.random() - 0.5) * 2;
        priceValue += priceChange;

        chartData.labels.push(xValue);
        chartData.datasets[0].data.push(priceValue);

        if (chartData.labels.length > 30) {
          chartData.labels.shift();
          chartData.datasets[0].data.shift();
        }

        myChart.update();

        xValue++;
      }
    }

    document.getElementById('startSimulation').addEventListener('click', () => {
      if (isSimulating) {
        clearInterval(simulationInterval);
        document.getElementById('status').innerText = 'Simulation Stopped';
        isSimulating = false;
      } else {
        simulationInterval = setInterval(simulateData, 1000);
        document.getElementById('status').innerText = 'Simulation Running';
        isSimulating = true;
      }
    });

    document.getElementById('status').innerText = 'Simulation Not Started';

    function resetChart() {
      chartData.labels = [];
      chartData.datasets[0].data = [];
      xValue = 0;
      priceValue = 100;
      myChart.update();
    }

    function resetButton() {
      let resetBtn = document.createElement('button');
      resetBtn.textContent = 'Reset Chart';
      document.body.appendChild(resetBtn);
      resetBtn.addEventListener('click', resetChart);
    }

    resetButton();

    for (let i = 0; i < 30; i++) {
      chartData.labels.push(i);
      chartData.datasets[0].data.push(priceValue);
      priceValue += (Math.random() - 0.5) * 2;
    }

    myChart.update();
  </script>
</body>
</html>
