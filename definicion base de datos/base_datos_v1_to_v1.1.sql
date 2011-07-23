SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE  TABLE IF NOT EXISTS `eSM`.`Fin_Reto` (
  `id_Reto` INT(11) NOT NULL ,
  `Puntuacion_Final_Equipo_Local` INT(11) NOT NULL ,
  `Puntuacion_Final_Equipo_Visitante` INT(11) NOT NULL ,
  `nid_Equipo_Ganador` INT(11) NOT NULL ,
  INDEX `fk_Reto_Completado_Reto` (`id_Reto` ASC) ,
  UNIQUE INDEX `id_Reto_UNIQUE` (`id_Reto` ASC) ,
  INDEX `fk_Reto_Completado_Puntuacion_Equipo1` (`Puntuacion_Final_Equipo_Local` ASC) ,
  INDEX `fk_Reto_Completado_Puntuacion_Equipo2` (`Puntuacion_Final_Equipo_Visitante` ASC) ,
  CONSTRAINT `fk_Reto_Completado_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reto_Completado_Puntuacion_Equipo1`
    FOREIGN KEY (`Puntuacion_Final_Equipo_Local` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Reto_Completado_Puntuacion_Equipo2`
    FOREIGN KEY (`Puntuacion_Final_Equipo_Visitante` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Inicio_Reto` (
  `id_Reto` INT(11) NOT NULL ,
  `nid_Equipo_Local` INT(11) NOT NULL ,
  `nid_Equipo_Visitante` INT(11) NOT NULL ,
  `Puntuacion_Inicial_Equipo_Local` INT(11) NOT NULL ,
  `Puntuacion_Inicial_Equipo_Visitante` INT(11) NOT NULL ,
  INDEX `fk_Adversario_Reto` (`id_Reto` ASC) ,
  INDEX `fk_Adversario_Equipo_Local` (`nid_Equipo_Local` ASC) ,
  INDEX `fk_Adversario_Equipo_Visitante` (`nid_Equipo_Visitante` ASC) ,
  INDEX `fk_Adversario_Puntuacion_Equipo_Local` (`Puntuacion_Inicial_Equipo_Local` ASC) ,
  INDEX `fk_Adversario_Puntuacion_Equipo_Visitante` (`Puntuacion_Inicial_Equipo_Visitante` ASC) ,
  CONSTRAINT `fk_Adversario_Reto`
    FOREIGN KEY (`id_Reto` )
    REFERENCES `eSM`.`Reto` (`id_Reto` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
  CONSTRAINT `fk_Adversario_Equipo_Local`
    FOREIGN KEY (`nid_Equipo_Local` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Adversario_Equipo_Visitante`
    FOREIGN KEY (`nid_Equipo_Visitante` )
    REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Adversario_Puntuacion_Equipo_Local`
    FOREIGN KEY (`Puntuacion_Inicial_Equipo_Local` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Adversario_Puntuacion_Equipo_Visitante`
    FOREIGN KEY (`Puntuacion_Inicial_Equipo_Visitante` )
    REFERENCES `eSM`.`Puntuacion` (`nid_Equipo` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

DROP TABLE IF EXISTS `eSM`.`Adversario` ;

DROP TABLE IF EXISTS `eSM`.`Reto_Completado` ;

ALTER TABLE `eSM`.`Modalidad_Juego` ADD COLUMN `reglas` INT(11) NOT NULL DEFAULT 0  AFTER `tid` ;

ALTER TABLE `eSM`.`Reto` DROP COLUMN `Estatus` , DROP COLUMN `Fecha` , DROP COLUMN `Hora_Fin` , DROP COLUMN `Hora_Inicio` , DROP COLUMN `idReto` , ADD COLUMN `Estado` TINYINT(1) NULL DEFAULT NULL  AFTER `id_Modalidad_Juego_Temporada` , ADD COLUMN `Fecha_Fin` INT(11) NOT NULL  AFTER `id_Modalidad_Juego_Temporada` , ADD COLUMN `Fecha_Inicio` INT(11) NOT NULL  AFTER `id_Modalidad_Juego_Temporada` , ADD COLUMN `id_Reto` INT(11) NOT NULL AUTO_INCREMENT  FIRST 
, DROP PRIMARY KEY 
, ADD PRIMARY KEY (`id_Reto`, `id_Modalidad_Juego_Temporada`) 
, DROP INDEX `idReto_UNIQUE` 
, ADD UNIQUE INDEX `idReto_UNIQUE` (`id_Reto` ASC) ;

ALTER TABLE `eSM`.`Integrantes_Equipo` 
DROP PRIMARY KEY ;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
