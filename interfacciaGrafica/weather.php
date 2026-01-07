<html>
 	<head>
  		<title>Ardu Weather</title>
			<style type="text/css">
				img{
					height: 150px;
					width: 150px;
				}
				table, td{
					border-top: 1px solid green;
					border-collapse: collapse;
				}
				table{
					position: relative;
					width: 100%;
					font-family: "Comic Sans MS", cursive, sans-serif;
					font-size: 25px;
					letter-spacing: 2px;
					word-spacing: 2px;
					color: #000000;
					font-weight: 700;
					text-decoration: none;
					font-style: normal;
					font-variant: normal;
					text-transform: none;
				}
				td{
					width: 10%;
					align-content: center;
					text-align: center;
				}
			</style>
 	</head>
 	<body>
		<form action="<?php echo(basename($_SERVER['PHP_SELF'])); ?>"> <!-- form che invia a questa stessa pagina-->
			<table>
				<tr>
				<th><font size="2">FILTERS(ALL OPTIONALS):</font></th>
				<th>
					day/month/year  
					<input type="text" name="f-day" size="2" placeholder="<?php echo(date ("d")); ?>" > /
					<input type="text" name="f-month" size="2" placeholder="<?php echo(date ("m")); ?>" > /
					<input type="text" name="f-year" size="4" placeholder="<?php echo(date ("y")); ?>" >
				</th>
				<th>location:
					<input type="text" name="f-loc" placeholder="Rome" >
				</th>
				<th></th>
				<th></th>
				<th></th>
				<th></th>
				<th><input type="submit" value="Submit"></th>
				</tr>
			</table>
		</form> 
		<table>
			<!--NomeStazione;giorno;mese;anno;ora;minuti;sec;luminosità;temperatura;umidità;pressione-->
			<!-- IT/Venice,9,5,2019,14,5,23,100,19,54,1018 -->
			<thead>
				<tr><th>Day/Night</th><th>Location</th><th>Time</th><th>brightness</th><th>temperature</th><th>umidity</th><th>pressure</th></tr>
			</thead>
			<tbody>
				<?php
					//retriving get params
					$day_filter="";
					$month_filter="";
					$year_filter="";
					$loc_filter="";
					if(!empty($_GET['f-day']))$day_filter=$_GET['f-day'];
					if(!empty($_GET['f-month']))$month_filter=$_GET['f-month'];
					if(!empty($_GET['f-year']))$year_filter=$_GET['f-year'];
					if(!empty($_GET['f-loc']))$loc_filter=$_GET['f-loc'];
					$filename = '../server/ServerMeteoDeLuca/weather.conditions';
					// The nested array to hold all the arrays
					$weather = []; 
					// Open the file for reading
					if (($h = fopen("{$filename}", "r")) !== FALSE) {
						// Each line in the file is converted into an individual array that we call $data
						// The items of the array are comma separated
						while (($data = fgetcsv($h, 1000, ",")) !== FALSE) 
						{
							// Each individual array is being pushed into the nested array
							$weather[] = $data;		
						}
						// Close the file
						fclose($h);
					}
					//Building the table with the data extracted from the weather csv file
					$build="";
					foreach($weather as $row){ 
						if(//filters
							($day_filter=="" || $day_filter==$row[1])&&
							($month_filter=="" || $month_filter==$row[2])&&
							($year_filter=="" || strstr($row[3],$year_filter))&&
							($loc_filter=="" || strstr($row[0],$loc_filter))
						){ 
							$build .= '<tr>';
							if($row[4]>6&&$row[4]<19){ // day
								if($row[10]>1000){/* rainy or sunny based on the pressure */
									$build .= '<td><img src="day.png"></td>'; 
								}else {
									$build .= '<td><img src="day-cloudy.png"></td>';
								}
							}else{ //night
								if($row[10]>1000){    /* rainy or sunny based on the pressure */
									$build .= '<td><img src="night.png"></td>';
								}else{
									$build .= '<td><img src="night-cloudy.png"></td>';
								}
							}
							$build .= "<td>{$row[0]}</td>"; /* location */
							$build .= "<td>{$row[1]}/{$row[2]}/{$row[3]} {$row[4]}:{$row[5]}:{$row[6]}</td>"; /* time */
							$build .= "<td>{$row[7]}</td>"; /* light */
							$build .= "<td>{$row[8]}</td>"; /* temperature */
							$build .= "<td>{$row[9]}</td>"; /* umidity */
							$build .= "<td>{$row[10]}</td>"; /* pressure */
							$build .= "</tr> \n";
						}
					}
					echo ($build);
				?>
			</tbody>
		</table>
		<p style="text-align: center;width: 100%;font-family: 'Comic Sans MS', cursive, sans-serif;">
			<font style="text-align: center; width: 100%;"><i><b>Developed by Lorenzo De Luca 2019</b></i></font>
		</p>
 	</body>
</html>