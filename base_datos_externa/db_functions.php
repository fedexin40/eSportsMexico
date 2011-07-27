<?php

/**
 * Función para obtener los datos de una modalidad
 * teniendo como base el id_Modalidad_Juego_Temporada
 * devuelve un objeto con todas las características de una modalidad
 */
function get_modalidad_juego_temporada( $id_Modalidad_Juego_Temporada ){
	//dpm( $id_Modalidad_Juego_Temporada );
	
	db_set_active('eSM');
		$Modalidad_Juego_Temporada = db_fetch_object(db_query('SELECT * FROM {Modalidad_Juego} AS mj INNER JOIN {Modalidad_Juego_Temporada} AS mjt ON mj.{id_Modalidad_Juego} = mjt.{id_Modalidad_Juego} WHERE mjt.{id_Modalidad_Juego_Temporada} = %d', $id_Modalidad_Juego_Temporada));
	db_set_active('default');
	return $Modalidad_Juego_Temporada;
}


/**
 *	Función para obtener modalidad a través de la modalidad y la temporada
 *	activas
 */
	function get_modalidad_juego_temporada_tid_id_temporada( $tid_Modalidad_Juego, $id_Temporada )
	{

		
		db_set_active('eSM');
			$Modalidad_Juego_temporada = db_fetch_object( db_query( 'SELECT * FROM { Modalidad_Juego_Temporada } AS { mjt } INNER JOIN { Modalidad_Juego } as{ mj } ON { mjt.id_Modalidad_Juego = mj.id_Modalidad_Juego }  WHERE { mj.tid } = %d AND { mjt.id_Temporada } = %d', $tid_Modalidad_Juego, $id_Temporada ) );
		db_set_active('default');
		
		return $Modalidad_Juego_Temporada;
		
	}

	
	
	/**
	 *	Función para obtener la modalidad de juego en la temporada a través del uid
	 */
	function get_modalidad_juego_temporada_user	( $user )
	{
		
		db_set_active( 'eSM' );
			$Modalidad_Juego_Temporada = db_fetch_object( db_query( 'SELECT * FROM { Jugador } as { j } INNER JOIN { Modalidad_Juego_Temporada } as { mjt } ON { j.id_Modalidad_Juego_Temporada = mjt.id_Modalidad_Juego_temporada } WHERE { j.uid } = %d ', $user->uid ) );
		db_set_active('default');
		
		return $Modalidad_Juego_Temporada;
		
		
		
	}



/**
 *	Función para obtener la modalidad de juego a través de el objeto $user
 */
	function get_modalidad_juego_user( $user )
	{
		
		db_set_active( 'eSM' );
			$Modalidad_Juego = db_fetch_object ( db_query( 'SELECT { * } FROM { Modalidad_Juego } AS { mj } INNER JOIN { Modalidad_Juego_Temporada } AS { mjt } ON { mj.id_Modalidad_Juego = mjt.id_Modalidad_Juego } INNER JOIN { Jugador } AS { j } ON { mjt.id_Modalidad_Juego_Temporada = j.id_Modalidad_Juego_Temporada } WHERE { j.uid = %d } ', $user->uid ) );
		db_set_active( 'default' );

		return $Modalidad_Juego;
		
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
	
	while ( $result = db_fetch_object( $modalidades_activas ) )
	{
		
		//dpm( $result );
		if ( $result->tid == $tid )
		{
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


