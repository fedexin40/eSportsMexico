<?php
/**
 * Funci�n para llamar a las funciones de cron para el m�dulo match
 */
function _match_cron_task () {
  //Llama a la funci�n para cerrar los partidos que no tengan resultado reportado
  _match_cron_reportes ();
  
  //Llama a la funci�n para cancelar los partidos que no se hayan aceptado
  _match_cron_cancelar_desafios_no_aceptados ();
}
/**
 * Funci�n para propagar los resultados en los desaf�os con m�s de X horas de programados
 * y para los que todav�a no se hayan reportado resultados de los dos participantes
 */
function _match_cron_reportes () {
  //Define el tiempo m�ximo de duraci�n de los partidos en espera de reporte de resultado
  $horas = 3; //Cambiar el valor de esta variable para cambiar el n�mero de horas de tolerancia para el reporte de resultados
  
  $fecha = time() - ($horas * 60 * 60);
  
  //Obtiene una lista de partidos con m�s de 3 horas de programado y que contin�an en estado de programado
  $retos = db_query('SELECT * FROM {eSM_Reto} AS r INNER JOIN {eSM_Inicio_Reto} AS ir ON r.nid_Reto = ir.nid_Reto
                       WHERE Estado = %d AND Fecha_Inicio <= %d', PROGRAMADO, $fecha);
  
  while ($reto = db_fetch_object($retos)) {
    $reporte_equipo_local = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Local);
    $reporte_equipo_visitante = _match_get_team_resultado($reto->nid_Reto, $reto->nid_Equipo_Visitante);
    $nid_Ganador = 0;
    
    //Resultado reportado �nicamente por el equipo local
    if(is_object($reporte_equipo_local)) {
      if ($reporte_equipo_local->Resultado == VICTORIA) {
        //Resultado coherente indicando que gan� el equipo local
        $nid_Ganador = $reto->nid_Equipo_Local;
      }
      elseif ($reporte_equipo_local->Resultado == DERROTA) {
        //Resultado coherente indicando que gan� el equipo visitante
        $nid_Ganador = $reto->nid_Equipo_Visitante;
      }
    }
    
    //Resultado reportado �nicamente por el equipo visitante
    if(is_object($reporte_equipo_visitante)) {
      if ($reporte_equipo_visitante->Resultado == VICTORIA) {
        //Resultado coherente indicando que gan� el equipo local
        $nid_Ganador = $reto->nid_Equipo_Visitante;
      }
      elseif ($reporte_equipo_visitante->Resultado == DERROTA) {
        //Resultado coherente indicando que gan� el equipo visitante
        $nid_Ganador = $reto->nid_Equipo_Local;
      }
    }
      
    //Si no hay un ganador definido, entonces el desaf�o debe ir al estado de conflicto
    if ($nid_Ganador == 0) {
      _match_update_match($reto->nid_Reto, CONFLICTO);
      //Notifica a los capitanes de los equipos que el reto se fue a estado de conflicto por no haber reportado resultado      
      //Primero obtenemos los datos de los capitanes de los equipos  
      $home_team = node_load(array('nid' => $reto->nid_Equipo_Local));
      $away_team = node_load(array('nid' => $reto->nid_Equipo_Visitante));
      
      $home_capitan = user_load(array('uid' => $home_team->uid));
      $away_capitan = user_load(array('uid' => $away_team->uid));
    
      rules_invoke_event('partido_conflicto', $home_capitan, $reto);
      rules_invoke_event('partido_conflicto', $home_capitan, $reto);
    }
    else {
      _match_end_match($reto->nid_Reto, $nid_Ganador);
    }
  }
}
/**
 * Funci�n para cancelar de manera autom�tica los desaf�os no aceptados antes de 5min de la hora de inicio
 */
function _match_cron_cancelar_desafios_no_aceptados () {
  //Define el tiempo m�ximo de duraci�n de los partidos en espera de reporte de resultado
  $minutos = 5; //Cambiar el valor de esta variable para cambiar el n�mero de horas de tolerancia para el reporte de resultados
  
  $fecha = time() + ($minutos * 60);
  
  //Obtiene una lista de los desaf�os que todav�a no han sido aceptados
  $retos = db_query('SELECT * FROM {eSM_Reto} AS r INNER JOIN {eSM_Inicio_Reto} AS ir ON r.nid_Reto = ir.nid_Reto
                       WHERE Estado = %d AND Fecha_Inicio <= %d', CREADO, $fecha);
  
  while ($reto = db_fetch_object($retos)) {
    db_query('UPDATE {eSM_Reto} SET Estado = %d WHERE nid_Reto = %d', CANCELADO, $reto->nid_Reto);
  }
}