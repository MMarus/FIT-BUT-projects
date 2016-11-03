<?php

/**
 * This file is part of the Nette Framework (http://nette.org)
 * Copyright (c) 2004 David Grudl (http://davidgrudl.com)
 */

namespace Nette\DI\Config\Adapters;

use Nette;
use Nette\DI\Config\Helpers;
use Nette\DI\Statement;
use Nette\Neon;


/**
 * Reading and generating NEON files.
 */
class NeonAdapter extends Nette\Object implements Nette\DI\Config\IAdapter
{
	/** @internal */
	const INHERITING_SEPARATOR = '<', // child < parent
		PREVENT_MERGING = '!';

	/**
	 * Reads configuration from NEON file.
	 * @param  string  file name
	 * @return array
	 */
	public function load($file)
	{
		return $this->process((array) Neon\Neon::decode(file_get_contents($file)));
	}


	private function process(array $arr)
	{
		$res = array();
		foreach ($arr as $key => $val) {
			if (substr($key, -1) === self::PREVENT_MERGING) {
				if (!is_array($val) && $val !== NULL) {
					throw new Nette\InvalidStateException("Replacing operator is available only for arrays, item '$key' is not array.");
				}
				$key = substr($key, 0, -1);
				$val[Helpers::EXTENDS_KEY] = Helpers::OVERWRITE;

			} elseif (preg_match('#^(\S+)\s+' . self::INHERITING_SEPARATOR . '\s+(\S+)\z#', $key, $matches)) {
				if (!is_array($val) && $val !== NULL) {
					throw new Nette\InvalidStateException("Inheritance operator is available only for arrays, item '$key' is not array.");
				}
				list(, $key, $val[Helpers::EXTENDS_KEY]) = $matches;
				if (isset($res[$key])) {
					throw new Nette\InvalidStateException("Duplicated key '$key'.");
				}
			}

			if (is_array($val)) {
				$val = $this->process($val);

			} elseif ($val instanceof Neon\Entity) {
				if ($val->value === Neon\Neon::CHAIN) {
					$tmp = NULL;
					foreach ($this->process($val->attributes) as $st) {
						$tmp = new Statement(
							$tmp === NULL ? $st->getEntity() : array($tmp, ltrim($st->getEntity(), ':')),
							$st->arguments
						);
					}
					$val = $tmp;
				} else {
					$tmp = $this->process(array($val->value));
					$val = new Statement($tmp[0], $this->process($val->attributes));
				}
			}
			$res[$key] = $val;
		}
		return $res;
	}


	/**
	 * Generates configuration in NEON format.
	 * @return string
	 */
	public function dump(array $data)
	{
		$tmp = array();
		foreach ($data as $name => $secData) {
			if ($parent = Helpers::takeParent($secData)) {
				$name .= ' ' . self::INHERITING_SEPARATOR . ' ' . $parent;
			}
			$tmp[$name] = $secData;
		}
		array_walk_recursive(
			$tmp,
			function (& $val) {
				if ($val instanceof Statement) {
					$val = NeonAdapter::statementToEntity($val);
				}
			}
		);

		return "# generated by Nette\n\n" . Neon\Neon::encode($tmp, Neon\Neon::BLOCK);
	}


	/**
	 * @return Neon\Entity
	 * @internal
	 */
	public static function statementToEntity(Statement $val)
	{
		array_walk_recursive(
			$val->arguments,
			function (& $val) {
				if ($val instanceof Statement) {
					$val = NeonAdapter::statementToEntity($val);
				}
			}
		);
		if (is_array($val->entity) && $val->entity[0] instanceof Statement) {
			return new Neon\Entity(
				Neon\Neon::CHAIN,
				array(
					self::statementToEntity($val->entity[0]),
					new Neon\Entity('::' . $val->entity[1], $val->arguments)
				)
			);
		} else {
			return new Neon\Entity($val->entity, $val->arguments);
		}
	}

}
