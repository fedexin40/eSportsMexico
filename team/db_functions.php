<?php



//Función que recibe un identificador de juego y temporada
//y devuelve un arreglo con los datos de todos los equipos inscritos
function get_equipos( $id_Modalidad_Juego_Temporada ) {
    db_set_active('eSM');
        $result = (db_query('SELECT {nid_Equipo}, {Abreviacion} FROM {Equipo}
			    WHERE {id_Modalidad_Juego_Temporada} = %d AND {Estado} = %d',
			    $id_Modalidad_Juego_Temporada, ACTIVO));
    db_set_active('default');
    
    $equipos = array( );
    
    while ( $equipo = db_fetch_array( $result ) )
    {
      
       $equipos[ $equipo[ 'nid_Equipo' ] ] = $equipo;
    }
    
    return is_null( $equipos ) ? NULL : $equipos ;
}

/**
  * Función que devuelve el nid del equipo para los que el usuario es capitán
  * devuelve NULL si el usuario no es capitán de ningún equipo en la modalidad evaluada
  * recibe el uid del usuario que se quiere verificar y el id_Modalidad_Juego_Temporada
 */
function is_capitan($uid, $id_Modalidad_Juego_Temporada) {
  
    $equipos = get_equipos( $id_Modalidad_Juego_Temporada );
    
    if ( isset( $equipos ) )
    {
      foreach ( $equipos as $nid=>$equipo )
      {
        $eval = node_load( $equipo[ 'nid_Equipo' ] );
        if ( $eval->uid == $uid )
          return $eval;
      }
    }
    else
      return NULL;
}



/**
 *  Función que devuelve el objeto con características de un término a través de
 *  un tid y de una variable_modalidades almacenada en la tabla variables de drupal
 *  que contiene el vid de nuestro vocabulario
 */
function get_term_data( $tid )
{
    
    $term_data_set = db_query
    (
      "SELECT
      { vid }
      FROM
      { term_data }
      WHERE
      { tid } = %d
      AND
      { vid } = %d",
      arg( 2 ),
      variable_get( 'vocabulario_modalidades', NULL )
    );
    
    $term_data_set = db_fetch_object( $term_data_set );
    
    return $term_data_set;
    
}



/**
 * función para obtener los equipos inscritos
 * en una modalidad de la temporada activa
 * recibe el id_Modalidad_Juego_Temporada
 * devuelve el resultado de la consulta para que
 * se pueda procesar con db_fetch_object
 * o db_fetch_array
 */
function get_user_not_affiliated_teams( $user ) {
	db_set_active('eSM');
		$teams = db_query('SELECT * FROM {Equipo} AS e INNER JOIN {Puntuacion} AS p ON e.nid_Equipo = p.nid_Equipo WHERE {id_Modalidad_Juego_Temporada} = %d ORDER BY Puntuacion DESC', $id_Modalidad_Juego_Temporada);
	db_set_active('default');
	
	return $teams;
}