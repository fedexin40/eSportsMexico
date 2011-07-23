-- phpMyAdmin SQL Dump
-- version 3.4.3.1deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jul 23, 2011 at 10:57 AM
-- Server version: 5.1.58
-- PHP Version: 5.3.6-13

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `eSM`
--


--
-- Dumping data for table `Plataforma`
--

INSERT INTO `Plataforma` (`id_Plataforma`, `Nombre`) VALUES
(2, 'Pc'),
(3, 'PS3'),
(1, 'Xbox');


--
-- Dumping data for table `Modalidad_Juego`
--

INSERT INTO `Modalidad_Juego` (`id_Modalidad_Juego`, `id_Plataforma`, `Nombre`, `Nombre_Juego`, `Minimo_Jugadores`, `Maximo_Jugadores`, `tid`) VALUES
(1, 2, 'League of legends', 'League of legends', 5, 8, 103),
(2, 2, 'Halo reach', 'Halo reach', 4, 7, 104),
(3, 2, 'Starcraft', 'Starcraft', 8, 10, 105);



--
-- Dumping data for table `Mapa`
--

INSERT INTO `Mapa` (`id_Mapa`, `id_Modalidad_Juego`, `Nombre`) VALUES
(1, 3, 'Moon'),
(2, 2, 'Summoner''s riff'),
(3, 2, 'Earth');


--
-- Dumping data for table `Temporada`
--

INSERT INTO `Temporada` (`id_Temporada`, `Nombre`, `Fecha_Inicio`, `Fecha_Fin`, `Estado`) VALUES
(1, 'Primera', 1311051600, 1319000400, 1),
(2, 'Segunda', 1321682400, 1324274400, 0);


--
-- Dumping data for table `Modalidad_Juego_Temporada`
--

INSERT INTO `Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`, `id_Modalidad_Juego`, `id_Temporada`) VALUES
(1, 1, 1),
(2, 2, 1),
(3, 3, 2),
(4, 1, 2);


--
-- Dumping data for table `Equipo`
--

INSERT INTO `Equipo` (`nid_Equipo`, `id_Modalidad_Juego_Temporada`, `Abreviacion`) VALUES
(111, 2, 'EG'),
(112, 1, 'RK');


--
-- Dumping data for table `Jugador`
--

INSERT INTO `Jugador` (`id_Jugador`, `id_Modalidad_Juego_Temporada`, `uid`, `Fecha_Registro`, `Estado`) VALUES
(1, 2, 1, 1311224400, 2),
(2, 2, 8, 1311224400, 1),
(3, 1, 1, 1311224400, 2),
(4, 1, 11, 1311224400, 1),
(5, 2, 5, 1311224400, 1);

--
-- Dumping data for table `Integrantes_Equipo`
--

INSERT INTO `Integrantes_Equipo` (`nid_Equipo`, `id_Jugador`, `Estado`, `Mensaje`) VALUES
(111, 1, 3, NULL),
(112, 3, 3, NULL);

--
-- Dumping data for table `Modo_Juego`
--

INSERT INTO `Modo_Juego` (`id_Modo_Juego`, `id_Modalidad_Juego`, `Nombre`) VALUES
(1, 2, 'Deatchmatch'),
(2, 3, 'Capture the flag'),
(3, 1, 'Team player');


--
-- Dumping data for table `Puntuacion`
--

INSERT INTO `Puntuacion` (`nid_Equipo`, `Puntuacion`, `PJ`, `PG`, `Racha`, `TJ`, `TG`) VALUES
(111, 0, 0, 0, 0, 0, 0),
(112, 0, 0, 0, 0, 0, 0);

--
-- Dumping data for table `Reto`
--

INSERT INTO `Reto` (`id_Reto`, `id_Modalidad_Juego_Temporada`, `Fecha_Inicio`, `Fecha_Fin`, `nid_Equipo_Local`, `nid_Equipo_Visitante`, `Estado`) VALUES
(1, 1, 1301590000, 1301610200, 111, 112, 0);



/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
