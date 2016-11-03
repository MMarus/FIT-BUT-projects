<?php

#CST:xmarus05

//all KWs
$keywords = array("auto", "break", "case", "char", "const", "continue", "default", "do",
"double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long",
"register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
"union", "unsigned", "void", "volatile", "while","inline","_Imaginary","_Complex","_Bool","restrict");

//REGEX ignored chars
$others = "/\.\.\.|\|\[|\]|\:|\{|\}|\(|\)|\;/";

//REGEX ignore pointers
$pointers= "/(((un)?signed )?int|char|short|short|long|struct\s*[_a-zA-z][_a-zA-z0-9]*){1}\s*\*+\s*[_a-zA-Z][_a-zA-Z0-9]*/";

//REGEX get operators
$operatorsAndThings= "/([_a-zA-Z0-9]*?\s*)(\=\=|\<\<\=|\<\<|\<\=|\<|\|\||\|\=|\||\&\&|\&\=|\&|\^\=|\^|\+\+|\+\=|\+|\>\>\=|\>\>|\-\-|\-\>|\>\=|\>|\-\=|\-|\*\=|\*|\/\=|\/|\%\=|\%|\!\=|\!|\~|\.|\=)(\s*[_a-zA-Z0-9]*\s*)?/";
//REGEX get operators2
$regexOperators = "/\=\=|\<\<\=|\<\<|\<\=|\<|\|\||\|\=|\||\&\&|\&\=|\&|\^\=|\^|\+\+|\+\=|\+|\>\>\=|\>\>|\-\-|\-\>|\>\=|\>|\-\=|\-|\*\=|\*|\/\=|\/|\%\=|\%|\!\=|\!|\~|\.|\=/";

//Count operators
//@param $line - one line of processed file
//@return  count of found operators
function countOperators($line){
  global $DEBUG;
  global $others;
  global $pointers;
  global $operatorsAndThings;
  global $regexOperators;
  if($DEBUG) echo "-------------".$line."-------------------\n";
  $count = 0;
  $line = preg_replace($pointers, "", $line);
  $line = preg_replace($others, "", $line);
  $line = preg_replace("/\=\&/", "=", $line);
  $line = preg_replace("/[0-9]+\.[0-9]+/", "", $line);
  preg_match_all($regexOperators, $line, $data);
  //print_r($data[0]);

  foreach( $data[0] as $word){
      //echo "OPERATOR==========|".$word."|============\n";
      $count++;
  }
  return $count;
}

//Count Identifiers
//@param $line - one line of processed file
//@return  count of found Identifiers
function countId(&$line){
#  $line = preg_replace("/\s0[xX][ABCDEFLluUabcdef0-9]*/", "", $line);
#  $line = preg_replace("/\s[0-9]+[lL]?[Uu]/", "", $line);
  global $keywords;
  $count = 0;
  $regex = "/[_a-zA-Z][_a-zA-Z0-9]*/";
  preg_match_all($regex, $line, $data);
  //print_r($data[0]);

  foreach( $data[0] as $word){
    if(!in_array($word,$keywords,true)){
     //echo "IDENTFIKATOR ==".$word."==\n";
      if($word[0])
      $count++;
    }
    //echo ($word != "") ? var_dump($word)."\n" : "";
  }

  //$line = preg_replace("/[_a-zA-Z][_a-zA-Z0-9]*/", "", $line);

  //echo $line;
  return $count;
}

//Count Keywords
//@param $line - one line of processed file
//@return  count of found Keywords
function countKeywords($line){
  global $keywords;
  $count = 0;
  $regex = "/[_a-zA-Z][_a-zA-Z0-9]*/";
  preg_match_all($regex, $line, $data);

  foreach( $data[0] as $word){
    if(in_array($word,$keywords,true)){
      //echo "KEYWORD ===========".$word."===============\n";
      $count++;
    }
    //echo ($word != "") ? var_dump($word)."\n" : "";
  }
  return $count;
}


//Remove all macros in file
//@param $line - one line of processed file
//@return  void
function removeMacros(&$line){
  //Use som regex??
  $start = strpos($line, "#");
  static $multiLineM = false;

  if( $multiLineM !== false){
    $multiLineM = substr($line, -2,1) == "\\";
    //vynulujem
    $line = "";
    return;
  }

  if($start !== false){
    $multiLineM = substr($line, -2,1) == "\\";
    //Rozdelime na parts
    $parts = explode("#",$line);
    $line = $parts[0];
    return;
  }
}

//Remove all strings like ''
//@param $line - one line of processed file
//@return  void
function removeStrings0(&$line){
  //$line = preg_replace("/\\\n/", "", $line);
  //$line = str_replace("\\\n", "", $line);
//  echo $line."\n";
  $line = preg_replace("/\'[^\']*\'/", "", $line);
}

//Remove all strings like "...."
//@param $line - one line of processed file
//@return  void
function removeStrings(&$line){
  //var_dump($line);
  $length = strlen($line);

  static $multiLine = false;
  $coment = false;
  $str = $multiLine;
  $escape = false;
  $line2 = "";

  for($i = 0; $i < $length; $i++){
    if($coment == true){
      if($line[$i] == "*" && $line[$i+1] == "/"){
        $coment == false;
        $line2 .= $line[$i];
        $line2 .= $line[$i+1];
        $i++;
      }
      else
        $line2 .= $line[$i];
      continue;
    }
    switch($line[$i]){
      case "/":
        if($line[$i+1] == "/" && $str != true){
          $coment = true;
          return;
        }
        elseif($line[$i+1] == "*" && $str != true){
          $coment = true;
          $line2 .= ($str == false || $coment == true) ? $line[$i] : "";
          $i++;
        }
          break;
      case "\\":
        //escape = true kdyz je escape nastaveny predtym na false
        //var_dump("length = ".($length-1)." i = ".$i);
        $escape = ( ($i != ($length-2) ) && $escape == false && $str == false);
        //var_dump("escape ".$escape);
        //neni escapeovany a je posledny znak
        $multiLine = ($str == true && $escape != true && $i == ($length-2));
        //var_dump("multiline ".$multiLine);
        break;
      case "\"":
        $old = $str;
        //var_dump("old ".$old);
        $str = ($escape == false && $str == false);
        //var_dump("str ".$str);
        //vymaz posledny "
        if($old == true && $str == false)
          $line[$i] = "";

        $multiLine= $str;
        //$line2 += ($escape == true && $str == false) ? $line[$i] : "";
        break;
    }
    $line2 .= ($str == false || $coment == true) ? $line[$i] : "";
    //echo $line2."\n";
  }
  $line = $line2;
}


//Remove all basicComments - //
//@param $line - one line of processed file
//@return  count of commented chars
function basicComment(&$line){

  $start = strpos($line, "//");
  //var_dump($start);
  $length = strlen($line);
  static $multiLine1 = false;

  if( $multiLine1 !== false){
    $multiLine1 = substr($line, -2,1) == "\\";
    //vynulujem
    $line = "";
    return $length;
  }

  if($start !== false){
    //echo "---ONE LINE---".$line."\n";
    $multiLine1 = substr($line, -2,1) == "\\";
    //Rozdelime na parts
    //$line = substr_replace($line, '', $start, $end+2);
    $parts = explode("//",$line);
    $line = $parts[0];
    return ($length - $start);
  }

}

//Remove all Comments like /**/
//@param $line - one line of processed file
//@return  count of commented chars
function isComment(&$line,$sameLine){

  $length = strlen($line);
  static $multiLine2 = false;
  $start = strpos($line, "/*");
  $end = strpos($line, "*/");
  //var_dump($multiLine2);

  if($multiLine2 !== false && $sameLine == true){
    return 0;
  }

  if($start !== false){
    if($end !== false){
      //vymazat komentovanu cast
      $line = substr_replace($line, '', $start, $end+2);
      return $end  - $start +2;
    }
    else{
      $multiLine2 = TRUE;
      $line = substr_replace($line, '', $start, $length);
      return $length - $start;
    }
  }
  else if($end !== false){
    if( $multiLine2 !== FALSE ){
      $multiLine2 = FALSE;
      //+1 lebo pozicia je od nuly,+1 su to dva znaky
      $line = substr_replace($line, '', 0, $end+2);
      return $end + 2;
    }
  }
  else{
    if( $multiLine2 !== FALSE ){
      $line = "";
      return $length;
    }
  }

}

//Find all matches with pattern
//@param $params["pattern"] - here is stored patter we are gonna look for
//@param $files- one line of processed file
//@return  count of matched words
//This one is a bit different then others
function findMatches($params, $files){
  global $DEBUG;
  /*@pattern = $params['pattern']*/
  $total = 0;

  foreach($files as $filename){
    $file = @fopen($filename,"r") or PrintErr(2);
    $matches = 0;

    while(! feof($file)){
      $line = fgets($file);
      if($DEBUG) echo $line;
      //TODO:najdi zhody
      $matches += substr_count($line, $params['pattern']);
    }
    fclose($file);
    pushDataToOutput($filename,$matches,$params["noPath"]);
    $total +=$matches;
  }
  return $total;
}

//Push acquired data to output array
//@param $Name = filename with realpath
//@param $count statistics we got from sources
//@param $noPath = says if we want to store path to files or not
//@return void, it populates array $OUTPUT with data
function pushDataToOutput($Name,$count,$nopath){
  global $OUTPUT;
  global $outLength;

  if($nopath == TRUE)
    $Name = basename($Name);

  $length = strlen($Name) + strlen((string)$count);

  $OUTPUT[] = array("name" => $Name,
                    "count" => $count
                  );
  if($length > $outLength)
    $outLength = $length;
}

//Print out all info we got
//@param $params["output"] - output file or stream
//@return void, it prints data to std
function WriteData($params){
  global $OUTPUT;
  global $outLength;
  $fw = @fopen($params['output'], "w") or PrintErr(3);

  //if($params['noPath'] == TRUE){
    function cmp($a, $b)
    {
      if($a["name"] == "CELKEM:")
        return 1;
      elseif( $b["name"] == "CELKEM:")
        return -1;
      //return strcasecmp($a["name"], $b["name"]);
      return strcmp($a["name"], $b["name"]);
    }

    usort($OUTPUT, 'cmp');
  //}

  foreach($OUTPUT as $file){
    $indent = $outLength+1 - strlen($file["name"]) - strlen($file["count"]);
    //echo $indent."\n";

    fwrite($fw, $file["name"].str_repeat(" ", $indent).$file["count"]."\n");
  }

  fclose($fw);
}

//Print out help
//@params $params["output"] output std or file
//@return void
function printHelp($params){
  $help = "Program for scanning and printing out statistics about C lang sources.\n";
  $help .= "Parameters:\n";
  $help .= "--help - print this help\n";
  $help .= "--input=<file/dirr> input file or dir\n";
  $help .= "--output=<file/dirr> output file or dir\n";
  $help .= "-k - print out count of keywords in files\n";
  $help .= "-o - print out count of operators in files\n";
  $help .= "-i - print out count of identifiers in files\n";
  $help .= "-w=<pattern> - print out count of word matched with pattern in files\n";
  $help .= "-c -  print out count of commented chars in files\n";
  $help .= "-p - print out data without filepaths\n";
  $help .= "--nosubdir - print out data about files only from current directory\n";
  $help .= "Author: Marek Marusic, xmarus05@stud.fit.vutbr.cz (c) 2015";

  $fw = @fopen($params['output'], "w") or PrintErr(3);
  fwrite($fw, $help."\n");
  fclose($fw);
  exit(0);
}
?>
