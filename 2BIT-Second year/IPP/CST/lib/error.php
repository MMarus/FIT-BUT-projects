<?php

#CST:xmarus05

//Print out errors and error codes
//@param $NUM - number of error
//@return void
function PrintErr( $NUM ){
  $Errors = array(
    "1" => "Wrong arguments entered",
    "2" => "Could not open input file",
    "3" => "Could not open output file",
    "4" => "Wrong format of input file",
    "10" => "",
    "100" => "",
  );

  file_put_contents( 'php://stderr', "ERROR:" . $Errors[$NUM] . "\n");
  exit($NUM);
}
?>
