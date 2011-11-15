<?php
/**
 * Función para mostrar las eliminatorias del torneo en la pantalla
 * en este momento únicamente están implementadas las eliminatorias para tornes simples
 */
function torneo_eliminatorias ($node) {
  $nid_Torneo = $node->nid;     //Obtiene el nid del torneo
  $settings = get_settings_torneo($nid_Torneo);   //Obtiene las configuraciones del torneo
  
  $image_path = drupal_get_path('module', 'torneo') . '/imagenes/';      //Obtiene la ruta de donde se van a tomar las imagenes
  
  drupal_add_css(drupal_get_path('module', 'torneo') . '/css/brackets.css');  //Añade el archivo css necesario para que se visualizen correctamente los brackets
  
  $totalteams = torneo_get_total_participantes($nid_Torneo);
  $totalbrackets = torneo_get_total_brackets($nid_Torneo);
  
  if ($totalbrackets == 0) {
    $output = '<div class=\"event_manager_status\"><p>El administrador no ha creado los brackets para este torneo todavia.</p></div>';
  }
  else {
    //Draw the brackets on the screen
    $header = array();
    $bracketSize = 2;
    $count = 2;
    while($totalteams > $bracketSize) {
      $count++;
      $bracketSize = $bracketSize * 2;
    }
    $lead=0;

    for ($table=1; $table<=$count; $table++) {
      $data = array();
	  if($table == $count) 
		$data['Titular'] = array('data' => array('<b><span id="final">Ganador</span> <img src="/' . $image_path . 'trophysmall.gif"></b>'), 'class' => 'header');
        
	  elseif($table == $count-1)
		$data['Titular'] = array('data' => array('<b><span style="color:#009933">Finalistas</span></b>'), 'class' => 'header');
        
	  elseif($table == $count-2)
		$data['Titular'] = array('data' => array('<b><span id="semi">Semi-Finalistas</span></b>'), 'class' => 'header');
        
	  else
	    $data['Titular'] = array('data' => array('<b><span style="color:#333333">Ronda '.$table.'</span></b>'), 'class' => 'header');
	  
	  $data['Espacio'] = array('data' => array('&nbsp;'));
	  
	  for ($y=0;$y<$lead;$y++) {
	    $data['Espacio'.$y] = array('data' => array('&nbsp;'));
	  }
	  
	  $front=$lead;
	  $lead=(2*$lead)+1;
	  for ($x=1; $x<=$bracketSize; $x++){
		if($x!=1){
			for ($y=0;$y<$lead;$y++) {
			  $data['Equipo'.$x.'Espacio'.$y] = array('data' => array('&nbsp;'));
			}
		}
		$team_info = _torneo_get_bracket_info($nid_Torneo, $table, $x);
        
		if ($team_info['nid_Equipo'] == 1) {
			$data['Equipo'.$x] = array('data' => array('&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span><i>KO</i></span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'), 'class' => 'bracketko', 'align' => 'center>');
		}
		elseif ($team_info['nid_Equipo'] == 0) {
			$data['Equipo'.$x] = array('data' => array('&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'), 'class' => 'bracketko', 'align' => 'center>');
		}
		else {
          $team = node_load(array('nid' => $team_info['nid_Equipo']));
          
          //$data['Equipo'.$x] = array('data' => array('&nbsp;<a href="node/' . $team->nid . '">' . $team->title . '</a>&nbsp;'), 'class' => 'bracket', 'align' => 'center>');
		  $data['Equipo'.$x] = array('data' => array('&nbsp;' . l($team->title, 'node/'. $team->nid ). '</a>&nbsp;'), 'class' => 'bracket', 'align' => 'center>');
		}
	    $middle=$lead+2;
	  }
	  $rows[] = array('data' => theme('table', $header, $data, array('id' => $table, 'class' => 'teams')));
	  $bracketSize = $bracketSize/2;
	//Tabla para hacer las líneas de los brackets
  	  if($table != $count){
  	    $data = array();
	    $data['Titular'] = array('data' => array('&nbsp;'), 'class' => 'header');
	    $data['Espacio'] = array('data' => array('&nbsp;'));
	    //$data['Espacio2'] = array('data' => array('&nbsp;'));
	    for ($w=1;$w<=$bracketSize;$w++) {
	  	  $mid = db_fetch_array(db_query("SELECT * FROM {eSM_Torneos_Brackets}
										 WHERE nid_Torneo = %d AND Ronda = %d AND Posicion = %d",
										 $nid, $table, $w*2));
       		
          for ($t=0;$t<$front;$t++) {
              $data['Equipo'.$w.'Front'.$t]= array('data' => array('&nbsp;'));
          }
          for ($t=0;$t<$middle;$t++) {
              $data['Equipo'.$w.'Middle'.$t]= array('data' => array('<a href="/node/'.$mid['mid'].'">&nbsp;</a>'), 'class' => 'color');
          }
          for ($t=0;$t<$front;$t++) {
              $data['Equipo'.$w.'Front1'.$t]= array('data' => array('&nbsp;'));
          }
          $data['Equipo'.$w.'Espacio']= array('data' => array('&nbsp;'));
	    }
	  $rows[] = array('data' => theme('table', $header, $data), 'class' => 'separador');
	  }
    }
    $output = theme('table', $header, array('data' => $rows), array('id' => 'Torneo_Brackets'));
  }
  return $output;

}