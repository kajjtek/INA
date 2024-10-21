SET NAMES utf8mb4;
SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

DROP SCHEMA IF EXISTS solvrorecrutation;
CREATE SCHEMA solvrorecrutation;
USE solvrorecrutation;

CREATE TABLE ingredients (
  ingredient_id int auto_increment,
  name varchar(255) not null,
  description varchar(255) not null,
  alcohol bool not null,
  image BLOB,
  PRIMARY KEY  (ingredient_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE cocktails_ingredients (
  cocktail_id int NOT NULL,
  name varchar(255) not null,
  ml int not null,
  alcohol bool not null,
  ingredient_id int not null,
  primary key(cocktail_id),
  foreign key(cocktail_id) references cocktails(cocktail_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

CREATE TABLE cocktails (
  cocktail_id int auto_increment,
  name varchar(255) not null,
  category varchar(32) NOT NULL,
  instructions varchar(255) NOT NULL,
  primary key(cocktail_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

