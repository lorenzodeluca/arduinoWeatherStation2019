<html>
    <head>
        <title>ArduWeather by Lorenzo De Luca</title>
        <style type="text/css">
            #map{
                width: 100%;
                height: 100%;
                background: url(stationSelector.jpg) no-repeat fixed; 
                z-index: 1;
            }
            html { 
                background: url(sfondo.png) no-repeat center center fixed; 
                -webkit-background-size: cover;
                -moz-background-size: cover;
                -o-background-size: cover;
                background-size: cover;
            }
            #Venice{
                position: absolute;
                align-content: center;
                top:160px;
                left: 480px;
                width: 200px;
                height: 200px;
                z-index: 2;
            }
            #Rome{
                position: absolute;
                align-content: center;
                top:400px;
                left: 480px;
                width: 200px;
                height: 200px;
                z-index: 2;
            }
            .station{
                width: 100px;
                height: 100px;
                z-index: 2;
            }
        </style>
    </head>
    <body>
        <!-- search for all the city in the csv file and display them-->
        <div id="map"  onclick="window.location.href = 'weather.php?f-loc=IT';">
        </div>
        <div id="city">
            <div id="Venice" >
                <img src="station.png" class="station" onclick="window.location.href = 'weather.php?f-loc=Venice';">
            </div>
            <div id="Rome">
                <img src="station.png" class="station" onclick="window.location.href = 'weather.php?f-loc=Rome';">
            </div>
        </div>
    </body>
</html>