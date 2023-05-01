const colors = Highcharts.getOptions().colors;

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
          menuItems: [
            "viewFullscreen",
            "viewData",
            "separator",
            "printChart",
            "separator",
            "downloadCSV",
            "downloadXLS",
          ],
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

var speed_chart = createChart(
  "speed-chart",
  "Скорость вращения двигателя",
  -3000,
  3000,
  [
    { name: "Максимальная", color: colors[2] },
    { name: "Текущая", color: colors[0] },
  ]
);

var acceleration_chart = createChart(
  "acceleration-chart",
  "Ускорение",
  0,
  3000,
  [
    { name: "Двигатель", color: colors[0] },
    { name: "Гироскоп", color: colors[3] },
  ]
);

var gyro_chart = createChart("gyro-chart", "Угол поворота", -180, 180, [
  { name: "Двигатель", color: colors[0] },
  { name: "Гироскоп", color: colors[5] },
]);
