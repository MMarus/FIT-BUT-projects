<?php

#CST:xmarus05

//Load libs
include 'lib/construct.php';
include 'lib/files.php';
include 'lib/error.php';
include 'lib/modes.php';

//array for stroing statistics about files
$OUTPUT= array();
//final length of the longest output
$outLength = 0;
//set to 1 if u wanna debug
$DEBUG=0;

//Save actual working dir for future
$workingDir = dirname(__FILE__);

if($DEBUG) echo $workingDir . "\n";

//parse arguments
$params = parse($argc);

//Print out help
if($params['help']){
  printHelp($params);
}

if($DEBUG) print_r($params);
if($DEBUG) var_dump($params['input']);

//Get all subfolders and all files
$files = findFiles($params['input'], $params['noSubdir']);
if($DEBUG) print_r($files);

//Find data by arguments
findStuff($params, $files);

//Print out data we found
WriteData($params);
?>
