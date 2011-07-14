-- phpMyAdmin SQL Dump
-- version 3.4.3deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jun 29, 2011 at 12:03 PM
-- Server version: 5.1.57
-- PHP Version: 5.3.6-12

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `eSM`
--

-- --------------------------------------------------------

--
-- Table structure for table `Adversarios`
--

CREATE TABLE IF NOT EXISTS `Adversarios` (
  `id_Reto` int(11) NOT NULL,
  `id_Retador` int(11) NOT NULL,
  `id_Retado` int(11) NOT NULL,
  PRIMARY KEY (`id_Reto`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Cat_Plataforma`
--

CREATE TABLE IF NOT EXISTS `Cat_Plataforma` (
  `id_Plat` int(11) NOT NULL,
  `Plataforma` varchar(8) NOT NULL,
  PRIMARY KEY (`id_Plat`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Equipos`
--

CREATE TABLE IF NOT EXISTS `Equipos` (
  `idEquipo` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `Rol` int(11) NOT NULL,
  PRIMARY KEY (`idEquipo`),
  KEY `Inscripcion` (`idEquipo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Jugador`
--

CREATE TABLE IF NOT EXISTS `Jugador` (
  `idEquipo` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `id_ModalidadTemporada` int(11) NOT NULL,
  `Fecha_Reg` date NOT NULL,
  PRIMARY KEY (`idEquipo`),
  KEY `Puntos` (`idEquipo`),
  KEY `Modalidades` (`id_ModalidadTemporada`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Mapas`
--

CREATE TABLE IF NOT EXISTS `Mapas` (
  `id_Modalidad` int(11) NOT NULL,
  `id_Map` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`id_Modalidad`,`id_Map`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Modalidades`
--

CREATE TABLE IF NOT EXISTS `Modalidades` (
  `id_Modalidad` int(11) NOT NULL,
  `id_Plat` int(11) NOT NULL,
  `Nombre_Juego` varchar(20) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `Min_Jugadores` int(11) NOT NULL,
  PRIMARY KEY (`id_Modalidad`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Modalidad_Temporada`
--

CREATE TABLE IF NOT EXISTS `Modalidad_Temporada` (
  `id_ModalidadTemporada` int(11) NOT NULL,
  `id_Modalidad` int(11) NOT NULL,
  `id_Temp` int(11) NOT NULL,
  PRIMARY KEY (`id_ModalidadTemporada`),
  KEY `fk_reto` (`id_ModalidadTemporada`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Modos_Juego`
--

CREATE TABLE IF NOT EXISTS `Modos_Juego` (
  `id_Modalidad` int(11) NOT NULL,
  `id_Modo` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`id_Modalidad`,`id_Modo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Pruebas`
--

CREATE TABLE IF NOT EXISTS `Pruebas` (
  `id_Reto` int(11) NOT NULL,
  `Prueba_Jug1` varchar(100) DEFAULT NULL,
  `Prueba_Jug2` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id_Reto`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Puntuacion`
--

CREATE TABLE IF NOT EXISTS `Puntuacion` (
  `id_Equipo` int(11) NOT NULL,
  `Puntos` int(11) NOT NULL,
  `PJ` int(11) NOT NULL,
  `PG` int(11) NOT NULL,
  `Racha` int(11) NOT NULL,
  PRIMARY KEY (`id_Equipo`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Reglas`
--

CREATE TABLE IF NOT EXISTS `Reglas` (
  `id_Reto` int(11) NOT NULL,
  `id_Map` int(11) NOT NULL,
  `id_Modo` int(11) NOT NULL,
  PRIMARY KEY (`id_Reto`),
  KEY `Reto` (`id_Reto`),
  KEY `Modos` (`id_Modo`),
  KEY `Mapas` (`id_Map`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Reto_Completado`
--

CREATE TABLE IF NOT EXISTS `Reto_Completado` (
  `id_Reto` int(11) NOT NULL,
  `Pts_Jug1` int(11) NOT NULL,
  `Pts_Jug2` int(11) NOT NULL,
  `Ganador` int(11) NOT NULL,
  PRIMARY KEY (`id_Reto`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Temporada`
--

CREATE TABLE IF NOT EXISTS `Temporada` (
  `id_Temp` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `Fecha_Ini` int(40) DEFAULT NULL,
  `Fecha_Fin` int(40) DEFAULT NULL,
  PRIMARY KEY (`id_Temp`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Temporada`
--

INSERT INTO `Temporada` (`id_Temp`, `Nombre`, `Fecha_Ini`, `Fecha_Fin`) VALUES
(39, 'Commodo Commodo Dolor Huic Suscipere', 1309226325, 1309226325),
(30, 'Acsi Os Amet Tego Sudo', 1309226324, 1309226324),
(20, '', 0, 0),
(21, 'Segunda', 0, 0),
(22, 'Segunda', 0, 0),
(23, 'Segunda', 0, 0),
(24, 'Segunda', 0, 0),
(25, 'Segunda', 0, 0),
(26, 'Segundaaaaa', 0, -36000);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
