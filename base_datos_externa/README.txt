Definir nuevas conexiones
Por defecto, Drupal utiliza la base de datos definida en el archivo default/setting.php gracias a la variable global $db_url (), como sigue:
<?php
$db_url = 'mysql://user:passw@localhost/drupal _bd'; 
?>
En este caso, Drupal utilizará la BD llamada 'drupal _db', sobre la cual el usuario 'user' con la contraseña 'passw' tiene todos los derechos.
Podríamos también utilizar lo siguiente, lo que nos dará el mismo resultado:
<?php
$db_url['default'] = 'mysql://user:passw@localhost/drupal _bd'; 
?>
Ya lo han entendido, es aquí que vamos a añadir una nueva conexión hacía otra base de datos.
<?php

// Conexión por defecto de Drupal 
$db_url['default'] = 'mysql://user:passw@localhost/drupal _bd'; 
// Nuevas conexiones a otras BD externos 
$db_url['new'] = 'mysql:// newuser:newpassw@localhost/new _bd' ;
$db_url['db2'] = 'mysql://otheruser:passw2@localhost/other _bd' ;
?>
A partir de este momento, Drupal dispondrá de tres conexiones, 'default', 'new' y 'db2', hacía tres bases de datos distintas: 'drupal _bd',' new _bd',' other _bd'. Por defecto Drupal utilizará siempre la conexión definida por la variable $db_url['default']
Cambiar de conexión
Para cambiar de conexión y enviar una consulta hacía una otra base de datos, utilizaremos función db_set_active()
<?php

// Conectarse a otra BD, en este caso other _bd
db_set_active('db2');
// Enviar la consulta
$result=db_query('select idregistro from table_in_ other _bd  where idregistro<>0  order by idregistro');
// recuperar el resultado
while ( $obj = db_fetch_object ($result) ) {
    …            
}
// ¡OJO ! Volver a conectarse a la BD por defecto de Drupal
db_set_active('default');
?>
No se olvide de volver a conectarse a la base de datos por defecto de Drupal al finalizar el proceso, sino Drupal hará todas sus consultas hacía la BD externa, lo que nos llevaría a un error.
Podríamos también poner el resultado de la consulta directamente en el cache de Drupal, pero abordaremos esto en un próximo artículo.
Debido a que el “database abstraction layer” de Drupal utiliza los mismos nombre de función para cada base de datos, no podemos recurrir simultáneamente a distintos sistemas de gestión de base de datos, como Mysql y PostgreSQL al mismo tiempo. Si realmente lo necesita, puede consultar este artículo para más información: http://drupal.org/node/19522
