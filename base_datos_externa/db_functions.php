<?php

/**
 * Función para obtener los datos de una modalidad
 * teniendo como base el id_Modalidad_Juego_Temporada
 * devuelve un objeto con todas las características de una modalidad
 */
function get_modalidad_juego_temporada( $id_Modalidad_Juego_Temporada ){
	//dpm( $id_Modalidad_Juego_Temporada );
	
	db_set_active('eSM');
		$modalidad = db_fetch_object(db_query('SELECT * FROM {Modalidad_Juego} AS m INNER JOIN {Modalidad_Juego_Temporada} AS t ON m.{id_Modalidad_Juego} = t.{id_Modalidad_Juego} WHERE t.{id_Modalidad_Juego_Temporada} = %d', $id_Modalidad_Juego_Temporada));
	db_set_active('default');
	return $modalidad;
}


/**
 * Función para obtener todas las modalidades activas
 * en la temporada activa.
 * Devuelve NULL si no hay temporada activa
 * o si no hay ninguna modalidad activa.
 * o devuelve un arreglo de objetos conteniendo
 * todas las modalidades activas en la temporada.
 */
function get_modalidades_juego_temporada_activas (){
    $temporada = temporada_activa();   //Obtiene la temporada activa actual
    
    if (isset($temporada)) {
        db_set_active('eSM');
            $result = db_query('SELECT * FROM {Modalidad_Juego_Temporada} WHERE id_Temporada = %d', $temporada->id_Temporada);
        db_set_active('default');
        $count = 0;
        while ($modalidad_activa = db_fetch_object($result)) {
            ++$count;
            $modalidad = get_modalidad_juego_temporada($modalidad_activa->id_Modalidad_Juego_Temporada);
            if (isset($modalidad))
                $modalidades[] = $modalidad;
        }
        if ($count == 0)
            return NULL;
        else
            return $modalidades;
    }
    else {
        return NULL;
    }
}


/*
 *  Función para verificar si un término pertenece al vocabulario definido para las modalidad
 *  y si dicha modalidad está activa en la temporada actual
 *  Regresa un objeto con la modalidad si está activa o NULL si no está activa
 */
function term_is_active( $tid ) {
	$modalidad  = NULL;
	$temporada_activa = temporada_activa( );
	
	db_set_active ('eSM');
	$modalidades_activas = db_query('SELECT {tid}, {Maximo_Jugadores}, {id_Modalidad_Juego_Temporada} FROM {Modalidad_Juego} AS m INNER JOIN {Modalidad_Juego_Temporada} AS t ON {m.id_Modalidad_Juego} = {t.id_Modalidad_Juego}  WHERE {id_Temporada} = %d', $temporada_activa->id_Temporada);
	db_set_active ('default');
	
	while ( $result = db_fetch_object($modalidades_activas) ) {
		
		//dpm( $result );
		if ($result->tid == $tid) {
				$modalidad = $result;
				break;
		}
	}
	
	//drupal_set_message( $modalidad );
	
	return $modalidad;
}



/*
 * Función para obtener la temporada activa
 * devuelve un objeto con todas las características de la temproada activa
 * o NULL si no existe temporada activa
 */
function temporada_activa() {
	$temporada_activa = NULL;
	db_set_active ('eSM');
		$temporada_activa = db_fetch_object( db_query('SELECT * FROM {Temporada} WHERE {Estado} = %d', 1 ) );
	db_set_active ('default');
	return $temporada_activa;
}


