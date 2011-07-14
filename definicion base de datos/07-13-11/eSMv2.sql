SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `eSM` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;

USE `eSM`;

CREATE  TABLE IF NOT EXISTS `eSM`.`Adversario` (
  `id_Reto` INT(11) NOT NULL ,
  `id_Retador` INT(11) NOT NULL ,
  `id_Retado` INT(11) NOT NULL ,
  INDEX `fk_Adversario_Reto` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Adversario_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Disponibilidad_Dias` (
  `id_Jugador` INT(11) NOT NULL ,
  `Lunes` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Martes` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Miercoles` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Jueves` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Viernes` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Sabado` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Domingo` TINYINT(4) NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Jugador`) ,
  INDEX `fk_Fechas_Disponibles_Agente_Libre` (`id_Jugador` ASC) ,
  UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Dias_Disponibles_Agente_Libre`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Equipo` (
  `nid_Equipo` INT(11) NOT NULL ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `Abreviacion` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`nid_Equipo`) ,
  UNIQUE INDEX `nid_Equipo_UNIQUE` (`nid_Equipo` ASC) ,
  INDEX `fk_Equipo_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  CONSTRAINT `fk_Equipo_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Integrantes_Equipo` (
  `nid_Equipo` INT(11) NOT NULL ,
  `id_Jugador` INT(11) NOT NULL ,
  `Estado` SMALLINT(1) NOT NULL DEFAULT -1 ,
  INDEX `fk_Equipo_Jugador` (`id_Jugador` ASC) ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Integrantes_Equipo_Equipo` (`nid_Equipo` ASC) ,
  UNIQUE INDEX `Estado_UNIQUE` (`Estado` ASC) ,
  CONSTRAINT `fk_Equipo_Jugador`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Jugador` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Integrantes_Equipo_Equipo`
    FOREIGN KEY (`nid_Equipo` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Jugador` (
  `id_Jugador` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `uid` INT(11) NOT NULL ,
  `Fecha_Registro` INT(11) NOT NULL ,
  `Estado` INT(11) NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Jugador`) ,
  INDEX `fk_Jugador_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Jugador_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Mapa` (
  `id_Mapa` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT(11) NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_Mapa`, `id_Modalidad_Juego`) ,
  INDEX `Mapa` (`id_Mapa` ASC) ,
  INDEX `fk_Mapa_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  UNIQUE INDEX `id_Mapa_UNIQUE` (`id_Mapa` ASC) ,
  CONSTRAINT `fk_Mapa_Modalidad_Juego`
    FOREIGN KEY (`id_Modalidad_Juego` )
    REFERENCES `eSM`.`Modalidad_Juego` (`id_Modalidad_Juego` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Modalidad_Juego_Temporada` (
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT(11) NOT NULL ,
  `id_Temporada` INT(11) NOT NULL ,
  PRIMARY KEY (`id_Modalidad_Juego_Temporada`, `id_Modalidad_Juego`, `id_Temporada`) ,
  INDEX `fk_Modalidad_Juego_Temporada_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  INDEX `fk_Modalidad_Juego_Temporada_Temporada` (`id_Temporada` ASC) ,
  UNIQUE INDEX `id_Modalidad_Juego_Temporada_UNIQUE` (`id_Modalidad_Juego_Temporada` ASC) ,
  CONSTRAINT `fk_Modalidad_Juego_Temporada_Modalidad_Juego`
    FOREIGN KEY (`id_Modalidad_Juego` )
    REFERENCES `eSM`.`Modalidad_Juego` (`id_Modalidad_Juego` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Modalidad_Juego_Temporada_Temporada`
    FOREIGN KEY (`id_Temporada` )
    REFERENCES `eSM`.`Temporada` (`id_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Modalidad_Juego` (
  `id_Modalidad_Juego` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Plataforma` INT(11) NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  `Nombre_Juego` VARCHAR(20) NOT NULL ,
  `Minimo_Jugadores` INT(11) NOT NULL ,
  `Maximo_Jugadores` INT(11) NOT NULL ,
  `tid` INT(11) NOT NULL ,
  PRIMARY KEY (`id_Modalidad_Juego`, `id_Plataforma`) ,
  INDEX `fk_Modalidad_Juego_Plataforma` (`id_Plataforma` ASC) ,
  UNIQUE INDEX `id_Modalidad_Juego_UNIQUE` (`id_Modalidad_Juego` ASC) ,
  UNIQUE INDEX `Nombre_UNIQUE` (`Nombre` ASC) ,
  UNIQUE INDEX `tid_UNIQUE` (`tid` ASC) ,
  CONSTRAINT `fk_Modalidad_Juego_Plataforma`
    FOREIGN KEY (`id_Plataforma` )
    REFERENCES `eSM`.`Plataforma` (`id_Plataforma` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Modo_Juego` (
  `id_Modo_Juego` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT(11) NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_Modo_Juego`, `id_Modalidad_Juego`) ,
  INDEX `fk_Modo_Juego_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  UNIQUE INDEX `id_Modo_Juego_UNIQUE` (`id_Modo_Juego` ASC) ,
  CONSTRAINT `fk_Modo_Juego_Modalidad_Juego`
    FOREIGN KEY (`id_Modalidad_Juego` )
    REFERENCES `eSM`.`Modalidad_Juego` (`id_Modalidad_Juego` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Plataforma` (
  `id_Plataforma` INT(11) NOT NULL AUTO_INCREMENT ,
  `Nombre` VARCHAR(8) NOT NULL ,
  PRIMARY KEY (`id_Plataforma`) ,
  UNIQUE INDEX `id_Plataforma_UNIQUE` (`id_Plataforma` ASC) ,
  UNIQUE INDEX `Nombre_UNIQUE` (`Nombre` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Prueba` (
  `id_Reto` INT(11) NOT NULL ,
  `Prueba_Jugador1` VARCHAR(100) NULL DEFAULT NULL ,
  `Prueba_Jugador2` VARCHAR(100) NULL DEFAULT NULL ,
  INDEX `fk_Prueba_Reto` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Prueba_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Puntuacion` (
  `nid_Equipo` INT(11) NOT NULL ,
  `Puntuacion` INT(11) NOT NULL ,
  `PJ` INT(11) NOT NULL ,
  `PG` INT(11) NOT NULL ,
  `Racha` INT(11) NOT NULL ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Puntuacion_Equipo` (`nid_Equipo` ASC) ,
  UNIQUE INDEX `nid_Equipo_UNIQUE` (`nid_Equipo` ASC) ,
  CONSTRAINT `fk_Puntuacion_Equipo`
    FOREIGN KEY (`nid_Equipo` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Regla` (
  `id_Reto` INT(11) NOT NULL ,
  `id_Mapa` INT(11) NOT NULL ,
  `id_Modo_Juego` INT(11) NOT NULL ,
  PRIMARY KEY (`id_Reto`, `id_Mapa`, `id_Modo_Juego`) ,
  INDEX `fk_Regla_Reto` (`id_Reto` ASC) ,
  INDEX `fk_Regla_Mapa` (`id_Mapa` ASC) ,
  INDEX `fk_Regla_Modo_Juego` (`id_Modo_Juego` ASC) ,
  CONSTRAINT `fk_Regla_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Regla_Mapa`
    FOREIGN KEY (`id_Mapa` )
    REFERENCES `eSM`.`Mapa` (`id_Mapa` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Regla_Modo_Juego`
    FOREIGN KEY (`id_Modo_Juego` )
    REFERENCES `eSM`.`Modo_Juego` (`id_Modo_Juego` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Reto_Completado` (
  `id_Reto` INT(11) NOT NULL ,
  `Puntuacion_Jugador1` INT(11) NOT NULL ,
  `Puntuacion_Jugador2` INT(11) NOT NULL ,
  `Ganador` INT(11) NOT NULL ,
  INDEX `fk_Reto_Completado_Reto` (`id_Reto` ASC) ,
  UNIQUE INDEX `id_Reto_UNIQUE` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Reto_Completado_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Reto` (
  `idReto` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `Fecha` INT(11) NOT NULL ,
  `Hora_Inicio` INT(11) NOT NULL ,
  `Hora_Fin` INT(11) NOT NULL ,
  `Estatus` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`idReto`, `id_Modalidad_Juego_Temporada`) ,
  INDEX `fk_Reto_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  UNIQUE INDEX `idReto_UNIQUE` (`idReto` ASC) ,
  CONSTRAINT `fk_Reto_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Temporada` (
  `id_Temporada` INT(11) NOT NULL AUTO_INCREMENT ,
  `Nombre` VARCHAR(45) NOT NULL ,
  `Fecha_Inicio` INT(11) NOT NULL ,
  `Fecha_Fin` INT(11) NOT NULL ,
  `Estado` TINYINT(1) NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Temporada`) ,
  UNIQUE INDEX `id_Temporada_UNIQUE` (`id_Temporada` ASC) ,
  UNIQUE INDEX `Nombre_UNIQUE` (`Nombre` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

ALTER TABLE `eSM`.`Disponibilidad_Horas` DROP COLUMN `id_Agente_Libre` , ADD COLUMN `id_Jugador` INT(11) NOT NULL  FIRST , DROP FOREIGN KEY `id_Agente_Libre_Horas` ;

ALTER TABLE `eSM`.`Disponibilidad_Horas` 
  ADD CONSTRAINT `fk_Disponibilidad_Horas_Agente_Libre`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE
, ADD INDEX `fk_Disponibilidad_Horas_Agente_Libre` (`id_Jugador` ASC) 
, DROP INDEX `id_Agente_Libre_Horas` 
, DROP PRIMARY KEY 
, ADD PRIMARY KEY (`id_Jugador`) 
, DROP INDEX `id_Agente_Libre_UNIQUE` 
, ADD UNIQUE INDEX `id_Agente_Libre_UNIQUE` (`id_Jugador` ASC) ;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
