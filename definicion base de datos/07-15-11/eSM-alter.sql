SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

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

DROP TABLE IF EXISTS `eSM`.`Disponibilidad_Dias` ;

ALTER TABLE `eSM`.`Integrantes_Equipo` 
DROP INDEX `Estado_UNIQUE` ;

ALTER TABLE `eSM`.`Agente_Libre` DROP FOREIGN KEY `fk_Agente_Libre_Jugador` ;

ALTER TABLE `eSM`.`Agente_Libre` 
  ADD CONSTRAINT `fk_Agente_Libre_Jugador`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Jugador` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE;

ALTER TABLE `eSM`.`Disponibilidad_Horas` DROP FOREIGN KEY `fk_Disponibilidad_Horas_Agente_Libre` ;

ALTER TABLE `eSM`.`Disponibilidad_Horas` 
  ADD CONSTRAINT `fk_Disponibilidad_Horas_Agente_Libre`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
