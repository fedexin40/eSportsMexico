Módulos para manejar la liga de eSports México con Drupal
Programadores: Agustín Cruz Lozano
               Irmin Nadir
               
Estructura de submódulos
  1. Base_Datos_Externa: Módulo core donde residen todas las funciones principales
  2. Team: Módulo para controlar y administrar la creación de equipos
  3. Match: Módulo para controlar y administrar las partidas.
  4. Ranking: Módulo para controlar y administrar las puntuaciones de equipos

Instalación:

Para instalar es necesario crear la base de datos externa definida en los archivos .sql de la carpeta "Definición base de datos"
el archivo con el que se generó la base de datos inicial (completa) para el servidor de pruebas fue el de la versión del día 07-23-11
a partir de ahí se empezaron a generar únicamente archivos para actualizar la base de datos al último estado de la base diseñada.


Desarrollo:

Existe un conjunto de funciones útiles para recolectar información de la base de datos. Si existen o se requieren, deben encontrarse dentro de
un archivo que lleva por nombre db_functions.php.
Para llamarlas desde otro módulo, deben incluirse primero dentro del ámbito de la función pertinente a través de el método siguiente:
module_load_include( $type, $module, $name = NULL );
Ejemplo:
Para usar las funciones del archivo db_functions de tipo php, perteneciente al ḿodulo base_datos_externa se hará de la siguiente manera:
module_load_include( 'php', 'base_datos_externa', 'db_functions' );