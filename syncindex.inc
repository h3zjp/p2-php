<?php
// p2 -  ���C�ɃX���̓���

require_once("./brdctl_class.inc");

//================================================
// �ǂݍ���
//================================================
//favlistfile�t�@�C�����Ȃ���ΏI��
if (!file_exists($syncfile)) {
	return;
}

//favlistfile�ǂݍ���;
$lines = @file($syncfile);

//board�ǂݍ���
$_current = BrdCtl::read_brd_online();

//================================================
// ����
//================================================

//���X�g��P���z��ɕϊ�
$current = array();
foreach ($_current as $brdmenu) {
	foreach ($brdmenu->categories as $category) {
		foreach ($category->menuitas as $ita) {
			$current[] = "{$ita->host}<>{$ita->bbs}";
		}
	}
}

//�f�[�^�̓���
$neolines = array();
foreach ($lines as $line) {
	$data = explode('<>', rtrim($line));
	if (preg_match('/^\w+\.(2ch\.net|bbspink\.com)$/', $data[10], $matches)) {
		$grep_pattern = '/^\w+\.' . preg_quote($matches[1], '/') . '<>' . preg_quote($data[11], '/') . '$/';
	} else {
		$neolines[] = $line;
		continue;
	}
	$findline = preg_grep($grep_pattern, $current);
	if ($findline) {
		$newdata = explode('<>', rtrim(array_shift($findline)));
		$data[10] = $newdata[0];
		$data[11] = $newdata[1];
		$neolines[] = implode('<>', $data) . "\n";
	} else {
		$neolines[] = $line;
	}
}

//================================================
//��������
//================================================
if (serialize($lines) != serialize($neolines)) {
	$fp = @fopen($syncfile, 'wb') or die("Error: $syncfile ���X�V�ł��܂���ł���");
	foreach ($neolines as $l) {
		fputs($fp, $l);
	}
	fclose($fp);
	$sync_ok = true;
} else {
	$sync_ok = false;
}

?>