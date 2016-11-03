<?php

#CST:xmarus05

//Parse arguments
//@param $argc
//return $params array with parameters
function parse($argc){

  global $DEBUG;
  global $workingDir;

  //Parse arguments
  $shortOpts  = "";
  $shortOpts .= "k";  // Required value
  $shortOpts .= "o";  // Required value
  $shortOpts .= "i";  // Required value
//  $shortOpts .= "k";  // Required value
  $shortOpts .= "w:"; // These options do not accept values
  $shortOpts .= "c"; // These options do not accept values
  $shortOpts .= "p"; // These options do not accept values

  $longOpts  = array(
      "help",         // Print help
      "input::",      // Input file or dir (optional)
      "nosubdir",     // Print help
      "output:",      // Output file (required????)
  );

  $options = getopt($shortOpts, $longOpts);

  //DEBUG
  if($DEBUG) echo $argc . "\n";

  if($DEBUG) var_dump($options);

  if($DEBUG) echo count($options) . "\n";
  //DEBUG


  //verify count of options
  if(count($options) != $argc - 1)
    PrintErr(1);

  //--help
  $data['help'] = FALSE;
  if ( isset($options['help']) ){
    if( $argc == 2 )
      $data['help'] = TRUE;
    else
      PrintErr(1);
  }
  //TODO: testni ci funguju ISO-8859-2 nazvy suborov
  //--input=
  if ( isset($options['input']) ){
    //var_dump($options['input']);
    //echo "./.".$options['input']."\n";
    $data['input'] = realpath($options['input']);
    //var_dump($options['input']);
  }
  else{
    if($DEBUG) echo $workingDir . "\n";
    $data['input'] = $workingDir;
  }

  //--nosubdir

  $data['noSubdir']=FALSE;
  if( isset($options['nosubdir']) ){
    //Ak je zadany iba jeden subor tak nemoze byt --nosubdir pouzity
    if(is_file($data['input']))
      PrintErr(1);

    $data['noSubdir']=TRUE;
  }

  //TODO: testni ci funguju ISO-8859-2 nazvy suborov
  //--output
  if( isset($options['output']) ){
    $data['output'] = $options['output'];
  }
  else{
    $data['output'] = "php://stdout";
  }

  $data['noPath'] = FALSE;
  //-p (basename)
  if( isset($options['p']) ){
    $data['noPath'] = TRUE;
  }

  $data['mode'] = "none";

  if( isset($options['k']) ){
    if ( $data['mode'] == "none" ){
      $data['mode'] = "k";
      //alebo zavolat rovno funkciu ?
    }
    else
      PrintErr(1);
  }

  if( isset($options['o']) ){
    if ( $data['mode'] == "none" ){
      $data['mode'] = "o";
      //alebo zavolat rovno funkciu ?
    }
    else
      PrintErr(1);
  }

  if( isset($options['i']) ){
    if ( $data['mode'] == "none" ){
      $data['mode'] = "i";
      //alebo zavolat rovno funkciu ?
    }
    else
      PrintErr(1);
  }

  if( isset($options['w']) ){
    if ( $data['mode'] == "none" ){
      $data['mode'] = "w";
      $data['pattern'] = $options['w'];
      //alebo zavolat rovno funkciu ?
    }
    else
      PrintErr(1);
  }

  if( isset($options['c']) ){
    if ( $data['mode'] == "none" ){
      $data['mode'] = "c";
      //alebo zavolat rovno funkciu ?
    }
    else
      PrintErr(1);
  }

  if( $data['mode'] == "none" && ! $data['help'] )
    PrintErr(1);
  elseif ( $data['mode'] != "none" && $data['help'] )
    PrintErr(1);
  return $data;
}

?>
