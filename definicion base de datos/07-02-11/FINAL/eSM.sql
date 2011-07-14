-- phpMyAdmin SQL Dump
-- version 3.4.3deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jul 02, 2011 at 08:35 PM
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
-- Table structure for table `Adversario`
--

CREATE TABLE IF NOT EXISTS `Adversario` (
  `id_Reto` int(11) NOT NULL,
  `id_Retador` int(11) NOT NULL,
  `id_Retado` int(11) NOT NULL,
  KEY `fk_Adversario_Reto` (`id_Reto`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Equipo`
--

CREATE TABLE IF NOT EXISTS `Equipo` (
  `idEquipo` int(11) NOT NULL,
  `uid` int(11) NOT NULL,
  `Rol` int(11) NOT NULL,
  PRIMARY KEY (`idEquipo`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Jugador`
--

CREATE TABLE IF NOT EXISTS `Jugador` (
  `idEquipo` int(11) NOT NULL,
  `id_Modalidad_Juego_Temporada` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `Fecha_Registro` int(11) NOT NULL,
  PRIMARY KEY (`idEquipo`,`id_Modalidad_Juego_Temporada`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `fk_Jugador_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`),
  KEY `fk_Jugador_Equipo` (`idEquipo`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Mapa`
--

CREATE TABLE IF NOT EXISTS `Mapa` (
  `id_Mapa` int(11) NOT NULL,
  `id_Modalidad_Juego` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`id_Mapa`,`id_Modalidad_Juego`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `Mapa` (`id_Mapa`),
  KEY `fk_Mapa_Modalidad_Juego` (`id_Modalidad_Juego`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Modalidad_Juego`
--

CREATE TABLE IF NOT EXISTS `Modalidad_Juego` (
  `id_Modalidad_Juego` int(11) NOT NULL,
  `id_Plataforma` int(11) NOT NULL,
  `Nombre_Juego` varchar(20) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `Minimo_Jugadores` int(11) NOT NULL,
  PRIMARY KEY (`id_Modalidad_Juego`,`id_Plataforma`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `fk_Modalidad_Juego_Plataforma` (`id_Plataforma`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Modalidad_Juego`
--

INSERT INTO `Modalidad_Juego` (`id_Modalidad_Juego`, `id_Plataforma`, `Nombre_Juego`, `Nombre`, `Minimo_Jugadores`) VALUES
(111, 111, 'ABC', 'DEF', 20);

-- --------------------------------------------------------

--
-- Table structure for table `Modalidad_Juego_Temporada`
--

CREATE TABLE IF NOT EXISTS `Modalidad_Juego_Temporada` (
  `id_Modalidad_Juego_Temporada` int(11) NOT NULL,
  `id_Modalidad_Juego` int(11) NOT NULL,
  `id_Temporada` int(11) NOT NULL,
  PRIMARY KEY (`id_Modalidad_Juego_Temporada`,`id_Modalidad_Juego`,`id_Temporada`),
  KEY `fk_Modalidad_Juego_Temporada_Modalidad_Juego` (`id_Modalidad_Juego`),
  KEY `fk_Modalidad_Juego_Temporada_Temporada` (`id_Temporada`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Modo_Juego`
--

CREATE TABLE IF NOT EXISTS `Modo_Juego` (
  `id_Modo_Juego` int(11) NOT NULL,
  `id_Modalidad_Juego` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`id_Modo_Juego`,`id_Modalidad_Juego`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `fk_Modo_Juego_Modalidad_Juego` (`id_Modalidad_Juego`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Plataforma`
--

CREATE TABLE IF NOT EXISTS `Plataforma` (
  `id_Plataforma` int(11) NOT NULL,
  `Nombre` varchar(8) NOT NULL,
  PRIMARY KEY (`id_Plataforma`),
  UNIQUE KEY `Nombre` (`Nombre`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Plataforma`
--

INSERT INTO `Plataforma` (`id_Plataforma`, `Nombre`) VALUES
(113, 'FHG'),
(112, 'FLY'),
(111, 'ZZZ');

-- --------------------------------------------------------

--
-- Table structure for table `Prueba`
--

CREATE TABLE IF NOT EXISTS `Prueba` (
  `id_Reto` int(11) NOT NULL,
  `Prueba_Jugador1` varchar(100) DEFAULT NULL,
  `Prueba_Jugador2` varchar(100) DEFAULT NULL,
  KEY `fk_Prueba_Reto` (`id_Reto`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Puntuacion`
--

CREATE TABLE IF NOT EXISTS `Puntuacion` (
  `id_Equipo` int(11) NOT NULL,
  `Puntuacion` int(11) NOT NULL,
  `PJ` int(11) NOT NULL,
  `PG` int(11) NOT NULL,
  `Racha` int(11) NOT NULL,
  KEY `fk_Puntuacion_Equipo` (`id_Equipo`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Regla`
--

CREATE TABLE IF NOT EXISTS `Regla` (
  `id_Reto` int(11) NOT NULL,
  `id_Mapa` int(11) NOT NULL,
  `id_Modo_Juego` int(11) NOT NULL,
  PRIMARY KEY (`id_Reto`,`id_Mapa`,`id_Modo_Juego`),
  KEY `fk_Regla_Reto` (`id_Reto`),
  KEY `fk_Regla_Mapa` (`id_Mapa`),
  KEY `fk_Regla_Modo_Juego` (`id_Modo_Juego`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Reto`
--

CREATE TABLE IF NOT EXISTS `Reto` (
  `idReto` int(11) NOT NULL,
  `id_Modalidad_Juego_Temporada` int(11) NOT NULL,
  `Fecha` int(11) NOT NULL,
  `Hora_Inicio` int(11) NOT NULL,
  `Hora_Fin` int(11) NOT NULL,
  `Estatus` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`idReto`,`id_Modalidad_Juego_Temporada`),
  KEY `fk_Reto_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Reto_Completado`
--

CREATE TABLE IF NOT EXISTS `Reto_Completado` (
  `id_Reto` int(11) NOT NULL,
  `Puntuacion_Jugador1` int(11) NOT NULL,
  `Puntuacion_Jugador2` int(11) NOT NULL,
  `Ganador` int(11) NOT NULL,
  UNIQUE KEY `id_Reto_UNIQUE` (`id_Reto`),
  KEY `fk_Reto_Completado_Reto` (`id_Reto`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Table structure for table `Temporada`
--

CREATE TABLE IF NOT EXISTS `Temporada` (
  `id_Temporada` int(11) NOT NULL AUTO_INCREMENT,
  `Nombre` varchar(45) NOT NULL,
  `Fecha_Inicio` int(11) NOT NULL,
  `Fecha_Fin` int(11) NOT NULL,
  PRIMARY KEY (`id_Temporada`),
  UNIQUE KEY `Nombre` (`Nombre`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=62 ;

--
-- Dumping data for table `Temporada`
--

INSERT INTO `Temporada` (`id_Temporada`, `Nombre`, `Fecha_Inicio`, `Fecha_Fin`) VALUES
(1, '', 1294920000, 1301572800),
(2, 'Nombre', 1302775200, 1325332800);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Adversario`
--
ALTER TABLE `Adversario`
  ADD CONSTRAINT `fk_Adversario_Reto` FOREIGN KEY (`id_Reto`) REFERENCES `Reto` (`idReto`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Jugador`
--
ALTER TABLE `Jugador`
  ADD CONSTRAINT `fk_Jugador_Equipo` FOREIGN KEY (`idEquipo`) REFERENCES `Equipo` (`idEquipo`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Jugador_Modalidad_Juego_Temporada` FOREIGN KEY (`id_Modalidad_Juego_Temporada`) REFERENCES `Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Mapa`
--
ALTER TABLE `Mapa`
  ADD CONSTRAINT `fk_Mapa_Modalidad_Juego` FOREIGN KEY (`id_Modalidad_Juego`) REFERENCES `Modalidad_Juego` (`id_Modalidad_Juego`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Constraints for table `Modalidad_Juego`
--
ALTER TABLE `Modalidad_Juego`
  ADD CONSTRAINT `fk_Modalidad_Juego_Plataforma` FOREIGN KEY (`id_Plataforma`) REFERENCES `Plataforma` (`id_Plataforma`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Modalidad_Juego_Temporada`
--
ALTER TABLE `Modalidad_Juego_Temporada`
  ADD CONSTRAINT `fk_Modalidad_Juego_Temporada_Modalidad_Juego` FOREIGN KEY (`id_Modalidad_Juego`) REFERENCES `Modalidad_Juego` (`id_Modalidad_Juego`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Modalidad_Juego_Temporada_Temporada` FOREIGN KEY (`id_Temporada`) REFERENCES `Temporada` (`id_Temporada`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Modo_Juego`
--
ALTER TABLE `Modo_Juego`
  ADD CONSTRAINT `fk_Modo_Juego_Modalidad_Juego` FOREIGN KEY (`id_Modalidad_Juego`) REFERENCES `Modalidad_Juego` (`id_Modalidad_Juego`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Prueba`
--
ALTER TABLE `Prueba`
  ADD CONSTRAINT `fk_Prueba_Reto` FOREIGN KEY (`id_Reto`) REFERENCES `Reto` (`idReto`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Puntuacion`
--
ALTER TABLE `Puntuacion`
  ADD CONSTRAINT `fk_Puntuacion_Equipo` FOREIGN KEY (`id_Equipo`) REFERENCES `Equipo` (`idEquipo`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Regla`
--
ALTER TABLE `Regla`
  ADD CONSTRAINT `fk_Regla_Mapa` FOREIGN KEY (`id_Mapa`) REFERENCES `Mapa` (`id_Mapa`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Regla_Modo_Juego` FOREIGN KEY (`id_Modo_Juego`) REFERENCES `Modo_Juego` (`id_Modo_Juego`) ON DELETE NO ACTION ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_Regla_Reto` FOREIGN KEY (`id_Reto`) REFERENCES `Reto` (`idReto`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Reto`
--
ALTER TABLE `Reto`
  ADD CONSTRAINT `fk_Reto_Modalidad_Juego_Temporada` FOREIGN KEY (`id_Modalidad_Juego_Temporada`) REFERENCES `Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`) ON DELETE NO ACTION ON UPDATE CASCADE;

--
-- Constraints for table `Reto_Completado`
--
ALTER TABLE `Reto_Completado`
  ADD CONSTRAINT `fk_Reto_Completado_Reto` FOREIGN KEY (`id_Reto`) REFERENCES `Reto` (`idReto`) ON DELETE NO ACTION ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
