<?php



//Función que recibe un identificador de juego y temporada
//y devuelve un arreglo con los datos de todos los equipos inscritos
function get_equipos($id_Modalidad_Juego_Tempodada) {
    $result = (db_query('SELECT {nid_Equipo}, {Abreviacion} FROM {Equipo} WHERE {id_Modalidad_Juego_Temporada} = %d', $id_Modalidad_Juego_Tempodada));
    
    while ($equipo = db_fetch_array($result)) {
        $equipos[$equipo['nid']] = $equipo;
    }
    return $equipos;
}

//Función que devuelve el nid del equipo para los que el usuario es capitán
//recibe el uid del usuario que se quiere verificar y el id_Modalidad_Juego_Temporada
function is_capitan($uid, $id_Modalidad_Juego_Temporada) {
    $equipos = get_equipos($id_Modalidad_Juego_Temporada);

    foreach ($equipos as $nid=>$equipo) {
        $eval = node_load($nid);
        if ($eval->uid == $uid)
            return $eval->nid;
    }
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
    
    $term_data = db_fetch_object( $term_data_set );
    
}