<?php

#CST:xmarus05

//Find stuff from files
//@param @params = array with params
//@param $files files to process
//@return void , push data to $OUTPUT array
function findStuff($params, $files){
  global $DEBUG;
  //Vyhladavani slova --> vyhladava sa vsude i v comments macros atd...
  if( $params['mode'] == "w" ){
    //zpracuj soubory
    $totalMatches = findMatches($params, $files);
    pushDataToOutput("CELKEM:",$totalMatches,$params['noPath']);
    //  echo "CELKEM:\t".$totalMatches."\n";
  }
  else{
    $totalMatches = 0;
    //zacatek prace so subory
    foreach($files as $filename){
      if($DEBUG) echo $filename;
      $file = @fopen($filename,"r") or PrintErr(2);
      $commentCount = 0;
      $keywordCount = 0;
      $idCount = 0;
      $operatorsCount = 0;

      //Otvoreny subor
      while(! feof($file)){
        $line = fgets($file);
        #echo "+++++++++++".$line;
        //odstranit stringy, pred ratannim komentv( tie sa mozu vyskytnut v str)

        removeStrings0($line);
        //echo "str0+++++++++++".$line;
        removeStrings($line);
        //echo "after str:|".$line."\n";

        //vymazani komentu
        $commentCount += basicComment($line);
        $sameLine = false;
        $length = isComment($line,$sameLine);
        //var_dump($length);
        while($length > 0){
          #echo "after com:|".$line."\n";
          $sameLine = true;
          $commentCount += $length;
          if($DEBUG) echo "comment lenght = " . $length;
          if($DEBUG) echo "\n";
          if($DEBUG) var_dump($line);
          $length = isComment($line,$sameLine);
          //var_dump($length);
        }
        #var_dump($commentCount);
        //echo ($line != "") ? "............".$line : "";
        //Vymazani Makier, az po mazani/ ratani komentov
        //lebo za makrami sa mozu vyskytnut kommenty
        removeMacros($line);
        //echo ($line != "") ? $line."\n" : "";
        if($params['mode'] == "k"){
          $keywordCount += countKeywords($line);
        }
        if($params['mode'] == "i"){
          $idCount += countId($line);
        }
        if($params['mode'] == "o"){
          $operatorsCount += countOperators($line);
        }

      }

      //vymenit za case ?
      //bolo by dobre spravit jednotny priechod ^^
      switch ($params['mode']){
        case "c":
          pushDataToOutput($filename,$commentCount,$params['noPath']);
          $totalMatches += $commentCount;
          break;
        case "k":
          pushDataToOutput($filename,$keywordCount,$params['noPath']);
          $totalMatches += $keywordCount;
          break;
        case "i":
          pushDataToOutput($filename,$idCount,$params['noPath']);
          $totalMatches += $idCount;
          break;
        case "o":
          pushDataToOutput($filename,$operatorsCount,$params['noPath']);
          $totalMatches += $operatorsCount;
          break;
      }
      fclose($file);
    }

    //Celkem ....
    pushDataToOutput("CELKEM:",$totalMatches,$params['noPath']);
  }
}

//Find files from subdirs etc
//@param @directory or specific file
//@param $noSubdir - tells wether not to look for files in subdirs
//@return $files array of found files
function findFiles($directory, $noSubdir) {

  $extensions = array("h","c");
  $files = array ();

  if( is_file($directory) ){
    $files[] = $directory;
    return $files;
  }
  elseif ( is_dir($directory) ){
    if( $noSubdir == TRUE )
      $directories = array("$directory");
    else
      glob_recursive($directory, $directories);

    foreach($directories as $directory) {
      foreach($extensions as $extension) {
        foreach(glob("{$directory}/*.{$extension}") as $file) {
          if(!is_readable($file))
            PrintErr(21);
          $files[] = $file;
        }
      }
    }

    //asort($files);
    return $files;
  }
  else
    PrintErr(2);
}

function glob_recursive($directory, &$directories = array()) {
  foreach(glob($directory, GLOB_ONLYDIR | GLOB_NOSORT) as $folder) {
    $directories[] = $folder;
    glob_recursive("{$folder}/*", $directories);
  }
}
?>
