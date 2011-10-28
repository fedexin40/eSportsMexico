<?php

/**
 * Función para desplegar la vista de actividad de la comunidad en una página
 */
function taxonomy_term_comunity () {
		return views_embed_view('activity_log_stream', 'default');
}