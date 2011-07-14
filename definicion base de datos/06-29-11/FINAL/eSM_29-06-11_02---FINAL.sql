SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `eSM` DEFAULT CHARACTER SET latin1 ;
USE `eSM` ;

-- -----------------------------------------------------
-- Table `eSM`.`Plataforma`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Plataforma` (
  `id_Plataforma` INT NOT NULL ,
  `Plataforma` VARCHAR(8) NOT NULL ,
  PRIMARY KEY (`id_Plataforma`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modalidad_Juego`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Modalidad_Juego` (
  `id_Modalidad_Juego` INT NOT NULL ,
  `id_Plataforma` INT NOT NULL ,
  `Nombre_Juego` VARCHAR(20) NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  `Minimo_Jugadores` INT NOT NULL ,
  PRIMARY KEY (`id_Modalidad_Juego`, `id_Plataforma`) ,
  INDEX `fk_Modalidad_Juego_Plataforma` (`id_Plataforma` ASC) ,
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
  `id_Temporada` INT NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  `Fecha_Inicio` INT NOT NULL ,
  `Fecha_Fin` INT NOT NULL ,
  PRIMARY KEY (`id_Temporada`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modalidad_Juego_Temporada`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Modalidad_Juego_Temporada` (
  `id_Modalidad_Juego_Temporada` INT NOT NULL ,
  `id_Modalidad_Juego` INT NOT NULL ,
  `id_Temporada` INT NOT NULL ,
  PRIMARY KEY (`id_Modalidad_Juego_Temporada`, `id_Modalidad_Juego`, `id_Temporada`) ,
  INDEX `fk_Modalidad_Juego_Temporada_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  INDEX `fk_Modalidad_Juego_Temporada_Temporada` (`id_Temporada` ASC) ,
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
-- Table `eSM`.`Reto`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Reto` (
  `idReto` INT NOT NULL ,
  `id_Modalidad_Juego_Temporada` INT NOT NULL ,
  `Fecha` INT NOT NULL ,
  `Hora_Inicio` INT NOT NULL ,
  `Hora_Fin` INT NOT NULL ,
  `Estatus` TINYINT(1)  NULL ,
  PRIMARY KEY (`idReto`, `id_Modalidad_Juego_Temporada`) ,
  INDEX `fk_Reto_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  CONSTRAINT `fk_Reto_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Adversario`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Adversario` (
  `id_Reto` INT NOT NULL ,
  `id_Retador` INT NOT NULL ,
  `id_Retado` INT NOT NULL ,
  INDEX `fk_Adversario_Reto` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Adversario_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Reto_Completado`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Reto_Completado` (
  `id_Reto` INT NOT NULL ,
  `Puntuacion_Jugador1` INT NOT NULL ,
  `Puntuacion_Jugador2` INT NOT NULL ,
  `Ganador` INT NOT NULL ,
  INDEX `fk_Reto_Completado_Reto` (`id_Reto` ASC) ,
  UNIQUE INDEX `id_Reto_UNIQUE` (`id_Reto` ASC) ,
  CONSTRAINT `fk_Reto_Completado_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Mapa`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Mapa` (
  `id_Mapa` INT NOT NULL ,
  `id_Modalidad_Juego` INT NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_Mapa`, `id_Modalidad_Juego`) ,
  INDEX `Mapa` (`id_Mapa` ASC) ,
  INDEX `fk_Mapa_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
  CONSTRAINT `fk_Mapa_Modalidad_Juego`
    FOREIGN KEY (`id_Modalidad_Juego` )
    REFERENCES `eSM`.`Modalidad_Juego` (`id_Modalidad_Juego` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Modo_Juego`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Modo_Juego` (
  `id_Modo_Juego` INT NOT NULL ,
  `id_Modalidad_Juego` INT NOT NULL ,
  `Nombre` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`id_Modo_Juego`, `id_Modalidad_Juego`) ,
  INDEX `fk_Modo_Juego_Modalidad_Juego` (`id_Modalidad_Juego` ASC) ,
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
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Puntuacion`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Puntuacion` (
  `nid_Equipo` INT NOT NULL ,
  `Puntuacion` INT NOT NULL ,
  `PJ` INT NOT NULL ,
  `PG` INT NOT NULL ,
  `Racha` INT NOT NULL ,
  PRIMARY KEY (`nid_Equipo`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Jugador`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Jugador` (
  `id_Jugador` INT(11) NOT NULL AUTO_INCREMENT ,
  `id_Modalidad_Juego_Temporada` INT(11) NOT NULL ,
  `uid` INT(11) NOT NULL ,
  `Fecha_Registro` INT(11) NOT NULL ,
  `Estado` INT(11) NOT NULL ,
  PRIMARY KEY (`id_Jugador`) ,
  INDEX `fk_Jugador_Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` ASC) ,
  UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Jugador_Modalidad_Juego_Temporada`
    FOREIGN KEY (`id_Modalidad_Juego_Temporada` )
    REFERENCES `eSM`.`Modalidad_Juego_Temporada` (`id_Modalidad_Juego_Temporada` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
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
    REFERENCES `eSM`.`Reto` (`idReto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eSM`.`Equipo`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Equipo` (
  `nid_Equipo` INT(11) NOT NULL ,
  `id_Jugador` INT(11) NOT NULL ,
  PRIMARY KEY (`nid_Equipo`) ,
  INDEX `fk_Equipo_Jugador` (`id_Jugador` ASC) ,
  INDEX `fk_Equipo_Puntuacion` (`nid_Equipo` ASC) ,
  CONSTRAINT `fk_Equipo_Jugador`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Jugador` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Equipo_Puntuacion`
    FOREIGN KEY (`nid_Equipo` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
