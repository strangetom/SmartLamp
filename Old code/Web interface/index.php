<?php
$mode = $_GET['mode'];
if($mode == "rainbow") {
  $file = fopen("light.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "rainbow"}');
  fclose($file);
} 
else if ($mode == "warm") {
  $file = fopen("light.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "warm"}');
  fclose($file);
}
else if ($mode == "cold") {
  $file = fopen("light.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "cold"}');
  fclose($file);
}
else if ($mode == "off") {
  $file = fopen("light.json", "w") or die("can't open file");
  fwrite($file, '{"mode": "off"}');
  fclose($file);
}
$colour = $_GET['colour'];
?>

<html>
  <head>      
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    
    <title>LED for ESP8266</title>
   
    <script src="https://code.jquery.com/jquery-2.2.2.min.js"></script>
    <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/js/bootstrap.min.js"></script>
    <script src="dist/js/bootstrap-colorpicker.js"></script>

    <link href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css" rel="stylesheet">
    <link rel="stylesheet" href="//maxcdn.bootstrapcdn.com/font-awesome/4.3.0/css/font-awesome.min.css">
    <link href="dist/css/bootstrap-colorpicker.min.css" rel="stylesheet">

  </head>
  <body>
    <div class="row" style="margin-top: 20px;">
      <div class="col-md-8 col-md-offset-2">
        <a href="?mode=rainbow" class="btn btn-success btn-block btn-lg">Rainbow</a>
        <br />

        <a href="?mode=warm" class="led btn btn-warning btn-block btn-lg">Warm</a>
        <br />

        <a href="?mode=cold" class="led btn btn-info btn-block btn-lg">Cold</a>
        <br />

        <a href="?mode=off" class="led btn btn-danger btn-block btn-lg">Turn Off</a>
        <br />

        <div class="mode-status well" style="margin-top: 5px; text-align:center">
          <?php
            if($mode=="rainbow") {
              echo("Lamp colour is rainbow.");
            }
            else if ($mode=="warm") {
              echo("Lamp colour is warm.");
            }
            else if ($mode=="cold") {
              echo("Lamp colour is cold.");
            }
            else if ($mode=="off") {
              echo("Lamp colour is off.");
            }
            else {
              echo ("Do something.");
            }
          ?>
        </div>
      </div>
    </div>
  </body>
</html>

