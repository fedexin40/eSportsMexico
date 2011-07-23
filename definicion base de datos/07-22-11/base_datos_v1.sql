SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `eSM` DEFAULT CHARACTER SET latin1 ;
USE `eSM` ;

-- -----------------------------------------------------
-- Table `eSM`.`Plataforma`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Plataforma` (
  `id_Plataforma` INT NOT NULL AUTO_INCREMENT ,
  `Nombre` VARCHAR(8) NOT NULL ,
  PRIMARY KEY (`id_Plataforma`) ,
  UNIQUE INDEX `id_Plataforma_UNIQUE` (`id_Plataforma` ASC) ,
  UNIQUE INDEX `Nombre_UNIQUE` (`Nombre` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modalidad_Juego`
-- -----------------------------------------------------
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Temporada`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Temporada` (
  `id_Temporada` INT NOT NULL AUTO_INCREMENT ,
  `Nombre` VARCHAR(45) NOT NULL ,
  `Fecha_Inicio` INT NOT NULL ,
  `Fecha_Fin` INT NOT NULL ,
  `Estado` TINYINT(1) NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Temporada`) ,
  UNIQUE INDEX `id_Temporada_UNIQUE` (`id_Temporada` ASC) ,
  UNIQUE INDEX `Nombre_UNIQUE` (`Nombre` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modalidad_Juego_Temporada`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Modalidad_Juego_Temporada` (
  `id_Modalidad_Juego_Temporada` INT NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT NOT NULL ,
  `id_Temporada` INT NOT NULL ,
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Equipo`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Equipo` (
  `nid_Equipo` INT(11) NOT NULL ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `Abreviacion` VARCHAR(45) NULL DEFAULT NULL ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Equipo_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  UNIQUE INDEX `nid_Equipo_UNIQUE` (`nid_Equipo` ASC) ,
  CONSTRAINT `fk_Equipo_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Reto`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Reto` (
  `id_Reto` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `Fecha_Inicio` INT(11) NOT NULL ,
  `Fecha_Fin` INT(11) NOT NULL ,
  `nid_Equipo_Local` INT(11) NULL ,
  `nid_Equipo_Visitante` INT(11) NULL ,
  `Estado` TINYINT(1) NULL DEFAULT NULL ,
  PRIMARY KEY (`id_Reto`, `id_Modalidad_Juego_Temporada`) ,
  INDEX `fk_Reto_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  UNIQUE INDEX `idReto_UNIQUE` (`id_Reto` ASC) ,
  INDEX `fk_Reto_Equipo_Visitante` (`nid_Equipo_Local` ASC) ,
  INDEX `fk_Reto_Equipo_Local` (`nid_Equipo_Visitante` ASC) ,
  CONSTRAINT `fk_Reto_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reto_Equipo_Visitante`
    FOREIGN KEY (`nid_Equipo_Local` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Reto_Equipo_Local`
    FOREIGN KEY (`nid_Equipo_Visitante` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Adversario`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Adversario` (
  `id_Reto` INT NOT NULL ,
  `nid_Equipo_Local` INT NOT NULL ,
  `nid_Equipo_Visitante` INT NOT NULL ,
  `Puntuacion_Equipo_Local` VARCHAR(45) NULL ,
  `Puntuacion_Equipo_Visitante` VARCHAR(45) NULL ,
  INDEX `fk_Adversario_Reto` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Adversario_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Puntuacion`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Puntuacion` (
  `nid_Equipo` INT NOT NULL ,
  `Puntuacion` INT NOT NULL DEFAULT 0 ,
  `PJ` INT NOT NULL DEFAULT 0 ,
  `PG` INT NOT NULL DEFAULT 0 ,
  `Racha` INT NOT NULL DEFAULT 0 ,
  `TJ` INT NOT NULL DEFAULT 0 ,
  `TG` INT NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Puntuacion_Equipo` (`nid_Equipo` ASC) ,
  UNIQUE INDEX `nid_Equipo_UNIQUE` (`nid_Equipo` ASC) ,
  CONSTRAINT `fk_Puntuacion_Equipo`
    FOREIGN KEY (`nid_Equipo` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Reto_Completado`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Reto_Completado` (
  `id_Reto` INT NOT NULL ,
  `Puntuacion_Equipo1` INT NOT NULL ,
  `Puntuacion_Equipo2` INT NOT NULL ,
  `Ganador` INT NOT NULL ,
  INDEX `fk_Reto_Completado_Reto` (`id_Reto` ASC) ,
  UNIQUE INDEX `id_Reto_UNIQUE` (`id_Reto` ASC) ,
  INDEX `fk_Reto_Completado_Puntuacion_Equipo1` (`Puntuacion_Equipo1` ASC) ,
  INDEX `fk_Reto_Completado_Puntuacion_Equipo2` (`Puntuacion_Equipo2` ASC) ,
  CONSTRAINT `fk_Reto_Completado_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reto_Completado_Puntuacion_Equipo1`
    FOREIGN KEY (`Puntuacion_Equipo1` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reto_Completado_Puntuacion_Equipo2`
    FOREIGN KEY (`Puntuacion_Equipo2` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Mapa`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Mapa` (
  `id_Mapa` INT NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT NOT NULL ,
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modo_Juego`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Modo_Juego` (
  `id_Modo_Juego` INT NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego` INT NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_Modo_Juego`, `id_Modalidad_Juego`) ,
  INDEX `fk_Modo_Juego_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  UNIQUE INDEX `id_Modo_Juego_UNIQUE` (`id_Modo_Juego` ASC) ,
  CONSTRAINT `fk_Modo_Juego_Modalidad_Juego`
    FOREIGN KEY (`id_Modalidad_Juego` )
    REFERENCES `eSM`.`Modalidad_Juego` (`id_Modalidad_Juego` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Regla`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Regla` (
  `id_Reto` INT NOT NULL ,
  `id_Mapa` INT NOT NULL ,
  `id_Modo_Juego` INT NOT NULL ,
  PRIMARY KEY (`id_Reto`, `id_Mapa`, `id_Modo_Juego`) ,
  INDEX `fk_Regla_Reto` (`id_Reto` ASC) ,
  INDEX `fk_Regla_Mapa` (`id_Mapa` ASC) ,
  INDEX `fk_Regla_Modo_Juego` (`id_Modo_Juego` ASC) ,
  CONSTRAINT `fk_Regla_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Jugador`
-- -----------------------------------------------------
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Prueba`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Prueba` (
  `id_Reto` INT NOT NULL ,
  `Prueba_Jugador1` VARCHAR(100) NULL DEFAULT NULL ,
  `Prueba_Jugador2` VARCHAR(100) NULL DEFAULT NULL ,
  INDEX `fk_Prueba_Reto` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Prueba_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Integrantes_Equipo`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Integrantes_Equipo` (
  `nid_Equipo` INT(11) NOT NULL ,
  `id_Jugador` INT(11) NOT NULL ,
  `Estado` SMALLINT(1) NOT NULL DEFAULT -1 ,
  `Mensaje` VARCHAR(45) NULL DEFAULT NULL ,
  INDEX `fk_Equipo_Jugador` (`id_Jugador` ASC) ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Integrantes_Equipo_Equipo` (`nid_Equipo` ASC) ,
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Agente_Libre`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Agente_Libre` (
  `id_Jugador` INT(11) NOT NULL ,
  `Descripcion` VARCHAR(300) NOT NULL ,
  PRIMARY KEY (`id_Jugador`) ,
  INDEX `fk_Agente_Libre_Jugador` (`id_Jugador` ASC) ,
  UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Agente_Libre_Jugador`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Jugador` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Disponibilidad_Dias`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Disponibilidad_Dias` (
  `id_Jugador` INT(11) NOT NULL ,
  `Lunes` TINYINT NOT NULL DEFAULT 0 ,
  `Martes` TINYINT NOT NULL DEFAULT 0 ,
  `Miercoles` TINYINT NOT NULL DEFAULT 0 ,
  `Jueves` TINYINT NOT NULL DEFAULT 0 ,
  `Viernes` TINYINT NOT NULL DEFAULT 0 ,
  `Sabado` TINYINT NOT NULL DEFAULT 0 ,
  `Domingo` TINYINT NOT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Jugador`) ,
  INDEX `fk_Fechas_Disponibles_Agente_Libre` (`id_Jugador` ASC) ,
  UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Dias_Disponibles_Agente_Libre`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Disponibilidad_Horas`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Disponibilidad_Horas` (
  `id_Jugador` INT(11) NOT NULL ,
  `Morning` TINYINT NOT NULL DEFAULT 0 ,
  `Evening` TINYINT NOT NULL DEFAULT 0 ,
  `Night` TINYINT NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Jugador`) ,
  UNIQUE INDEX `id_Agente_Libre_UNIQUE` (`id_Jugador` ASC) ,
  INDEX `fk_Disponibilidad_Horas_Agente_Libre` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Disponibilidad_Horas_Agente_Libre`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Puntos_Desafio_Jugador_Inferior`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Puntos_Desafio_Jugador_Inferior` (
  `Diferencia` INT NOT NULL DEFAULT 0 ,
  `Puntos_Win` INT NOT NULL DEFAULT 0 ,
  `Puntos_Loss` INT NOT NULL DEFAULT 0 ,
  INDEX `Puntos_Desafio_Jugador_Inferior` (`Diferencia` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Puntos_Desafio_Jugador_Superior`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Puntos_Desafio_Jugador_Superior` (
  `Diferencia_Puntos` INT NOT NULL DEFAULT 0 ,
  `Puntos_Win` INT NOT NULL DEFAULT 0 ,
  `Puntos_Loss` INT NOT NULL DEFAULT 0 ,
  INDEX `Puntos_Desafio_Jugador_Superior` (`Diferencia_Puntos` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Puntuacion_Torneos`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Puntuacion_Torneos` (
  `Inscritos` INT(11) NOT NULL DEFAULT 0 ,
  `1` INT(11) NOT NULL DEFAULT 0 ,
  `2` INT(11) NOT NULL DEFAULT 0 ,
  `4` INT(11) NOT NULL DEFAULT 0 ,
  `8` INT(11) NOT NULL DEFAULT 0 ,
  `16` INT(11) NOT NULL DEFAULT 0 ,
  `32` INT(11) NOT NULL DEFAULT 0 ,
  `64` INT(11) NOT NULL DEFAULT 0 ,
  `128` INT(11) NOT NULL DEFAULT 0 ,
  `256` INT(11) NOT NULL DEFAULT 0 ,
  `512` INT(11) NOT NULL DEFAULT 0 ,
  `1024` INT(11) NOT NULL DEFAULT 0 ,
  INDEX `Torneos_Jugadores_Inscritos` (`Inscritos` ASC) )
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
