-- phpMyAdmin SQL Dump
-- version 3.4.3deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Jul 05, 2011 at 08:53 PM
-- Server version: 5.1.57
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

-- --------------------------------------------------------

--
-- Table structure for table `Adversario`
--

CREATE TABLE IF NOT EXISTS `Adversario` (
  `id_Reto` int(11) NOT NULL AUTO_INCREMENT,
  `id_Retador` int(11) NOT NULL,
  `id_Retado` int(11) NOT NULL,
  KEY `fk_Adversario_Reto` (`id_Reto`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Adversario`
--

INSERT INTO `Adversario` (`id_Reto`, `id_Retador`, `id_Retado`) VALUES
(111, 1110, 1111),
(112, 1120, 1121),
(113, 1130, 1131),
(114, 1140, 1141),
(115, 1150, 1151);

-- --------------------------------------------------------

--
-- Table structure for table `Equipo`
--

CREATE TABLE IF NOT EXISTS `Equipo` (
  `idEquipo` int(11) NOT NULL AUTO_INCREMENT,
  `uid` int(11) NOT NULL,
  `Rol` int(11) NOT NULL,
  PRIMARY KEY (`idEquipo`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Equipo`
--

INSERT INTO `Equipo` (`idEquipo`, `uid`, `Rol`) VALUES
(111, 1111, 1111),
(112, 1122, 1122),
(113, 1133, 1133),
(114, 1144, 1144),
(115, 1155, 1155);

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

--
-- Dumping data for table `Mapa`
--

INSERT INTO `Mapa` (`id_Mapa`, `id_Modalidad_Juego`, `Nombre`) VALUES
(93, 111, 'DFDFA'),
(111, 111, 'DFGFDG'),
(112, 112, 'FDGDFG'),
(114, 114, 'GCVBCV'),
(94, 111, 'MAPAUNO'),
(113, 113, 'SDSVCVBC'),
(115, 115, 'VVBNVB');

-- --------------------------------------------------------

--
-- Table structure for table `Modalidad_Juego`
--

CREATE TABLE IF NOT EXISTS `Modalidad_Juego` (
  `id_Modalidad_Juego` int(11) NOT NULL AUTO_INCREMENT,
  `id_Plataforma` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  `Nombre_Juego` varchar(25) NOT NULL,
  `Minimo_Jugadores` int(15) NOT NULL,
  PRIMARY KEY (`id_Modalidad_Juego`,`id_Plataforma`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `fk_Modalidad_Juego_Plataforma` (`id_Plataforma`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Modalidad_Juego`
--

INSERT INTO `Modalidad_Juego` (`id_Modalidad_Juego`, `id_Plataforma`, `Nombre`, `Nombre_Juego`, `Minimo_Jugadores`) VALUES
(111, 111, 'DEF', '', 0),
(112, 112, 'ASDASD', '', 0),
(113, 113, 'DFVXCVXC', '', 0),
(114, 114, 'XCVXCVX', '', 0),
(115, 115, 'FGSRTE', '', 0);

-- --------------------------------------------------------

--
-- Table structure for table `Modalidad_Juego_Temporada`
--

CREATE TABLE IF NOT EXISTS `Modalidad_Juego_Temporada` (
  `id_Modalidad_Juego_Temporada` int(11) NOT NULL AUTO_INCREMENT,
  `id_Modalidad_Juego` int(11) NOT NULL,
  `id_Temporada` int(11) NOT NULL,
  PRIMARY KEY (`id_Modalidad_Juego_Temporada`,`id_Modalidad_Juego`,`id_Temporada`),
  KEY `fk_Modalidad_Juego_Temporada_Modalidad_Juego` (`id_Modalidad_Juego`),
  KEY `fk_Modalidad_Juego_Temporada_Temporada` (`id_Temporada`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Modalidad_Juego_Temporada`
--

INSERT INTO `Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`, `id_Modalidad_Juego`, `id_Temporada`) VALUES
(111, 111, 111),
(112, 112, 112),
(113, 113, 113),
(114, 114, 114),
(115, 115, 115);

-- --------------------------------------------------------

--
-- Table structure for table `Modo_Juego`
--

CREATE TABLE IF NOT EXISTS `Modo_Juego` (
  `id_Modo_Juego` int(11) NOT NULL AUTO_INCREMENT,
  `id_Modalidad_Juego` int(11) NOT NULL,
  `Nombre` varchar(45) NOT NULL,
  PRIMARY KEY (`id_Modo_Juego`,`id_Modalidad_Juego`),
  UNIQUE KEY `Nombre` (`Nombre`),
  KEY `fk_Modo_Juego_Modalidad_Juego` (`id_Modalidad_Juego`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Modo_Juego`
--

INSERT INTO `Modo_Juego` (`id_Modo_Juego`, `id_Modalidad_Juego`, `Nombre`) VALUES
(111, 111, 'DSFSDFSDFSS'),
(114, 114, 'FGHFGHCVV'),
(113, 113, 'GFHFHFGHFG'),
(115, 115, 'HGHJGHJGHJGH'),
(112, 112, 'SSDFSDFSDFSD');

-- --------------------------------------------------------

--
-- Table structure for table `Plataforma`
--

CREATE TABLE IF NOT EXISTS `Plataforma` (
  `id_Plataforma` int(11) NOT NULL AUTO_INCREMENT,
  `Nombre` varchar(25) NOT NULL,
  PRIMARY KEY (`id_Plataforma`),
  UNIQUE KEY `Nombre` (`Nombre`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=136 ;

--
-- Dumping data for table `Plataforma`
--

INSERT INTO `Plataforma` (`id_Plataforma`, `Nombre`) VALUES
(115, 'ASDAASED'),
(114, 'ASDDfff'),
(112, 'FLYSAAAAAA'),
(113, 'qwqasdasd'),
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

--
-- Dumping data for table `Prueba`
--

INSERT INTO `Prueba` (`id_Reto`, `Prueba_Jugador1`, `Prueba_Jugador2`) VALUES
(111, 'FDGDFGDFG', 'DFGDFGDFGDFG'),
(112, 'DFGDFGDFG', 'DGDFGDFGDFG'),
(113, 'GFHFGH', 'GHVBNVBN'),
(114, 'BVNVBNVB', 'VBNVBNVN'),
(115, 'VBNVBNVBN', 'FGFGHF');

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
  UNIQUE KEY `id_Equipo` (`id_Equipo`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Puntuacion`
--

INSERT INTO `Puntuacion` (`id_Equipo`, `Puntuacion`, `PJ`, `PG`, `Racha`) VALUES
(111, 11111, 2222, 1111, 0),
(112, 2222, 5555, 3333, 0),
(113, 3333, 6666, 3333, 0),
(114, 4444, 7777, 3333, 0),
(115, 5555, 8888, 3333, 0);

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

--
-- Dumping data for table `Regla`
--

INSERT INTO `Regla` (`id_Reto`, `id_Mapa`, `id_Modo_Juego`) VALUES
(111, 111, 111),
(112, 112, 113),
(113, 113, 113),
(114, 114, 114),
(115, 115, 115);

-- --------------------------------------------------------

--
-- Table structure for table `Reto`
--

CREATE TABLE IF NOT EXISTS `Reto` (
  `idReto` int(11) NOT NULL AUTO_INCREMENT,
  `id_Modalidad_Juego_Temporada` int(11) NOT NULL,
  `Fecha` int(11) NOT NULL,
  `Hora_Inicio` int(11) NOT NULL,
  `Hora_Fin` int(11) NOT NULL,
  `Estatus` tinyint(1) DEFAULT NULL,
  PRIMARY KEY (`idReto`,`id_Modalidad_Juego_Temporada`),
  KEY `fk_Reto_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=116 ;

--
-- Dumping data for table `Reto`
--

INSERT INTO `Reto` (`idReto`, `id_Modalidad_Juego_Temporada`, `Fecha`, `Hora_Inicio`, `Hora_Fin`, `Estatus`) VALUES
(111, 111, 132674500, 674500, 680500, 0),
(112, 112, 132674500, 453699, 453766, 0),
(113, 113, 132453699, 239078, 231001, 0),
(114, 114, 132453699, 349056, 341001, 0),
(115, 115, 132453699, 234589, 234689, NULL);

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

--
-- Dumping data for table `Reto_Completado`
--

INSERT INTO `Reto_Completado` (`id_Reto`, `Puntuacion_Jugador1`, `Puntuacion_Jugador2`, `Ganador`) VALUES
(111, 1000, 2000, 1111),
(112, 1000, 2000, 2222),
(113, 1000, 2000, 3333),
(114, 1000, 2000, 4444),
(115, 1000, 2000, 5555);

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
  UNIQUE KEY `Nombre` (`Nombre`),
  UNIQUE KEY `Fecha_Inicio` (`Fecha_Inicio`),
  UNIQUE KEY `Fecha_Fin` (`Fecha_Fin`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=133 ;

--
-- Dumping data for table `Temporada`
--

INSERT INTO `Temporada` (`id_Temporada`, `Nombre`, `Fecha_Inicio`, `Fecha_Fin`) VALUES
(111, 'PRIMERAAAA', 1310328000, 1310329800),
(112, 'SEGUNDA', 1310335200, 1310337000),
(113, 'TERCERA', 1310342400, 1310344200),
(114, 'CUARTA', 1310349600, 1310351400),
(115, 'QUINTA', 1310356800, 1310358600),
(132, 'HDJTUSKSI', 1387713600, 1387720800);

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
