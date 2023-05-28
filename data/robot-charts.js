function createChart(renderTo, title, min, max, series) {
  return Highcharts.chart(renderTo, {
    chart: {
      type: "line",
      marginRight: 10,
      height: 350,
      style: {
        fontFamily: "unset",
      },
    },

    time: {
      useUTC: false,
    },

    title: {
      text: title,
    },

    xAxis: {
      type: "datetime",
    },

    yAxis: {
      title: {
        enabled: false,
      },
      min,
      max,
    },

    exporting: {
      enabled: true,
      buttons: {
        contextButton: {
          menuItems: ["viewFullscreen", "viewData", "printChart", "downloadCSV", "downloadXLS"],
        },
      },
      csv: {
        dateFormat: "%d.%m.%Y %H:%M:%S.%L",
        decimalPoint: ".",
        itemDelimiter: ";",
      },
    },

    accessibility: {
      enabled: false,
    },

    series,
  });
}

function appendChart(chart, index, x, y) {
  const series = chart.series[index];
  series.addPoint([x, y], true, series.points.length >= 300, false); // <- 300 = ~30 сек
}

function clearChart(chart) {
  chart.series.forEach(x => x.setData([], false));
  chart.redraw();
}

const colors = Highcharts.getOptions().colors;

var speed_chart = createChart("speed-chart", "Скорость вращения двигателя", -2000, 2000, [
  { name: "Заданная", color: colors[2] },
  { name: "Текущая", color: colors[0] },
]);

// var acceleration_chart = createChart("acceleration-chart", "Ускорение", 0, 100, [
//   { name: "Гироскоп X", color: colors[2], visible: false },
//   { name: "Гироскоп Y", color: colors[3], visible: false },
//   { name: "Гироскоп Z", color: colors[4] },
//   { name: "Двигатель", color: colors[0] },
// ]);

var gyro_chart = createChart("gyro-chart", "Угол поворота", 0, 360, [
  // { name: "Крен (X)", color1: colors[2], visible: false },
  // { name: "Тангаж (Y)", color1: colors[3], visible: false },
  // { name: "Рыскание (Z)", color1: colors[4], visible: false },
  { name: "Двигатель", color1: colors[0] },
  { name: "Робот", color1: colors[4], visible: false },
]);

function clearCharts() {
  clearChart(speed_chart);
  clearChart(acceleration_chart);
  clearChart(gyro_chart);
}
