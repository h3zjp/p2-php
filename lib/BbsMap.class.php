<?php
/* vim: set fileencoding=cp932 ai et ts=4 sw=4 sts=4 fdm=marker: */
/* mi: charset=Shift_JIS */

require_once P2_LIBRARY_DIR . '/p2util.class.php';
require_once P2_LIBRARY_DIR . '/filectl.class.php';

// {{{ class BbsMap

/**
 * BbsMapクラス
 *
 * 板-ホストの対応表を作成し、それに基づいてホストの同期を行う
 */
class BbsMap
{
    // {{{ getCurrentHost()

    /**
     * 最新のホストを取得する
     *
     * @param   string  $host   ホスト名
     * @param   string  $bbs    板名
     * @param   bool    $autosync   移転を検出したときに自動で同期するか否か
     * @return  string  板に対応する最新のホスト
     * @access  public
     * @static
     */
    function getCurrentHost($host, $bbs, $autosync = true)
    {
        global $_info_msg_ht;
        static $synced = false;

        // マッピング読み込み
        $map = BbsMap::_getMapping();
        if (!$map) {
            return $host;
        }
        $type = BbsMap::_detectHostType($host);

        // チェック
        if (isset($map[$type]) && isset($map[$type][$bbs])) {
            $new_host = $map[$type][$bbs]['host'];
            if ($host != $new_host && $autosync && !$synced) {
                // 移転を検出したらお気に板、お気にスレ、最近読んだスレを自動で同期
                $msg_fmt = '<p>rep2 info: ホストの移転を検出しました。(%s/%s → %s/%s)<br>';
                $msg_fmt .= 'お気に板、お気にスレ、最近読んだスレを自動で同期します。</p>';
                $_info_msg_ht .= sprintf($msg_fmt, $host, $bbs, $new_host, $bbs);
                BbsMap::syncFav();
                $synced = true;
            }
            $host = $new_host;
        }

        return $host;
    }

    // }}}
    // {{{ getBbsName()

    /**
     * 板名LONGを取得する
     *
     * @param   string  $host   ホスト名
     * @param   string  $bbs    板名
     * @return  string  板メニューに記載されている板名
     * @access  public
     * @static
     */
    function getBbsName($host, $bbs)
    {
        // マッピング読み込み
        $map = BbsMap::_getMapping();
        if (!$map) {
            return $bbs;
        }
        $type = BbsMap::_detectHostType($host);

        // チェック
        if (isset($map[$type]) && isset($map[$type][$bbs])) {
            $itaj = $map[$type][$bbs]['itaj'];
        } else {
            $itaj = $bbs;
        }

        return $itaj;
    }

    // }}}
    // {{{ syncBrd()

    /**
     * お気に板などのbrdファイルを同期する
     *
     * @param   string  $brd_path   brdファイルのパス
     * @return  void
     * @access  public
     * @static
     */
    function syncBrd($brd_path)
    {
        global $_conf, $_info_msg_ht;
        static $done = array();

        // {{{ 読込

        if (isset($done[$brd_path])) {
            return;
        }
        $lines = BbsMap::_readData($brd_path);
        if (!$lines) {
            return;
        }
        $map = BbsMap::_getMapping();
        if (!$map) {
            return;
        }
        $neolines = array();
        $updated = false;

        // }}}
        // {{{ 同期

        foreach ($lines as $line) {
            $setitaj = false;
            $data = explode("\t", rtrim($line, "\n"));
            $hoge = $data[0]; // 予備?
            $host = $data[1];
            $bbs  = $data[2];
            $itaj = $data[3];
            $type = BbsMap::_detectHostType($host);

            if (isset($map[$type]) && isset($map[$type][$bbs])) {
                $newhost = $map[$type][$bbs]['host'];
                if ($itaj === '') {
                    $itaj = $map[$type][$bbs]['itaj'];
                    if ($itaj != $bbs) {
                        $setitaj = true;
                    } else {
                        $itaj = '';
                    }
                }
            } else {
                $newhost = $host;
            }

            if ($host != $newhost || $setitaj) {
                $neolines[] = "{$hoge}\t{$newhost}\t{$bbs}\t{$itaj}\n";
                $updated = true;
            } else {
                $neolines[] = $line;
            }
        }

        // }}}
        // {{{ 書込

        if ($updated) {
            BbsMap::_writeData($brd_path, $neolines);
            $_info_msg_ht .= sprintf('<p>rep2 info: %s を同期しました。</p>', htmlspecialchars($brd_path, ENT_QUOTES));
        } else {
            $_info_msg_ht .= sprintf('<p>rep2 info: %s は変更されませんでした。</p>', htmlspecialchars($brd_path, ENT_QUOTES));
        }
        $done[$brd_path] = true;

        // }}}
    }

    // }}}
    // {{{ syncIdx()

    /**
     * お気にスレなどのidxファイルを同期する
     *
     * @param   string  $idx_path   idxファイルのパス
     * @return  void
     * @access  public
     * @static
     */
    function syncIdx($idx_path)
    {
        global $_conf, $_info_msg_ht;
        static $done = array();

        // {{{ 読込

        if (isset($done[$idx_path])) {
            return;
        }
        $lines = BbsMap::_readData($idx_path);
        if (!$lines) {
            return;
        }
        $map = BbsMap::_getMapping();
        if (!$map) {
            return;
        }
        $neolines = array();
        $updated = false;

        // }}}
        // {{{ 同期

        foreach ($lines as $line) {
            $data = explode('<>', rtrim($line, "\n"));
            $host = $data[10];
            $bbs  = $data[11];
            $type = BbsMap::_detectHostType($host);

            if (isset($map[$type]) && isset($map[$type][$bbs])) {
                $newhost = $map[$type][$bbs]['host'];
            } else {
                $newhost = $host;
            }

            if ($host != $newhost) {
                $data[10] = $newhost;
                $neolines[] = implode('<>', $data) . "\n";
                $updated = true;
            } else {
                $neolines[] = $line;
            }
        }

        // }}}
        // {{{ 書込

        if ($updated) {
            BbsMap::_writeData($idx_path, $neolines);
            $_info_msg_ht .= sprintf('<p>rep2 info: %s を同期しました。</p>', htmlspecialchars($idx_path, ENT_QUOTES));
        } else {
            $_info_msg_ht .= sprintf('<p>rep2 info: %s は変更されませんでした。</p>', htmlspecialchars($idx_path, ENT_QUOTES));
        }
        $done[$idx_path] = true;

        // }}}
    }

    // }}}
    // {{{ syncFav()

    /**
     * お気に板、お気にスレ、最近読んだスレを同期する
     *
     * @return  void
     * @access  public
     * @static
     */
    function syncFav()
    {
        global $_conf;
        BbsMap::syncBrd($_conf['favita_path']);
        BbsMap::syncIdx($_conf['favlist_file']);
        BbsMap::syncIdx($_conf['rct_file']);
    }

    // }}}
    // {{{ _getMapping()

    /**
     * 2ch公式メニューをパースし、板-ホストの対応表を作成する
     *
     * @return  array   site/bbs/(host,itaj) の多次元連想配列
     *                  ダウンロードに失敗したときは false
     * @access  private
     * @static
     */
    function _getMapping()
    {
        global $_conf, $_info_msg_ht;
        static $map = null;

        // {{{ 設定

        $bbsmenu_url = 'http://menu.2ch.net/bbsmenu.html';
        $map_cache_path = $_conf['pref_dir'] . '/p2_cache/host_bbs_map.txt';
        $map_cache_lifetime = 600; // TTLは少し短めに
        $errfmt = '<p>rep2 error: BbsMap: %s - %s をダウンロードできませんでした。</p>';

        // }}}
        // {{{ キャッシュ確認

        if (!is_null($map)) {
            return $map;
        } elseif (file_exists($map_cache_path)) {
            $mtime = filemtime($map_cache_path);
            $expires = $mtime + $map_cache_lifetime;
            if (time() < $expires) {
                $map_cahce = file_get_contents($map_cache_path);
                $map = unserialize($map_cahce);
                return $map;
            }
        }

        // }}}
        // {{{ メニューをダウンロード

        $params = array();
        if (isset($mtime)) {
            $params['requestHeaders'] = array('If-Modified-Since' => gmdate('D, d M Y H:i:s', $mtime) . ' GMT');
        }
        $req = &new HTTP_Request($bbsmenu_url, $params);
        $req->setMethod('GET');
        $err = $req->sendRequest(true);

        // エラーを検証
        if (PEAR::isError($err)) {
            $_info_msg_ht .= sprintf($errfmt, htmlspecialchars($err->getMessage()), htmlspecialchars($bbsmenu_url, ENT_QUOTES));
            if (file_exists($map_cache_path)) {
                return unserialize(file_get_contents($map_cache_path));
            } else {
                return false;
            }
        }

        // レスポンスコードを検証
        $code = $req->getResponseCode();
        if ($code == 304) {
            $map_cahce = file_get_contents($map_cache_path);
            $map = unserialize($map_cahce);
            touch($map_cache_path);
            return $map;
        } elseif ($code != 200) {
            $_info_msg_ht .= sprintf($errfmt, htmlspecialchars(strval($code)), htmlspecialchars($bbsmenu_url, ENT_QUOTES));
            if (file_exists($map_cache_path)) {
                return unserialize(file_get_contents($map_cache_path));
            } else {
                return false;
            }
        }

        $res_body = $req->getResponseBody();

        // }}}
        // {{{ パース

        $regex = '!<A HREF=http://(\w+\.(?:2ch\.net|bbspink\.com|machi\.to|mathibbs\.com))/(\w+)/(?: TARGET=_blank)?>(.+?)</A>!';
        preg_match_all($regex, $res_body, $matches, PREG_SET_ORDER);

        $map = array();
        foreach ($matches as $match) {
            $host = $match[1];
            $bbs  = $match[2];
            $itaj = $match[3];
            $type = BbsMap::_detectHostType($host);
            if (!isset($map[$type])) {
                $map[$type] = array();
            }
            $map[$type][$bbs] = array('host' => $host, 'itaj' => $itaj);
        }

        // }}}
        // {{{ キャッシュする

        $map_cache = serialize($map);
        if (FileCtl::file_write_contents($map_cache_path, $map_cache) === false) {
            $errmsg = sprintf('Error: cannot write file. (%s)', htmlspecialchars($map_cache_path, ENT_QUOTES));
            die($errmsg);
        }

        // }}}

        return $map;
    }

    // }}}
    // {{{ _readData()

    /**
     * 更新前のデータを読み込む
     *
     * @param   string  $path   読み込むファイルのパス
     * @return  array   ファイルの内容、読み出しに失敗したときは false
     * @access  private
     * @static
     */
    function _readData($path)
    {
        if (!file_exists($path)) {
            return false;
        }

        $lines = file($path);
        if (!$lines) {
            return false;
        }

        return $lines;
    }

    // }}}
    // {{{ _writeData()

    /**
     * 更新後のデータを書き込む
     *
     * @param   string  $path   書き込むファイルのパス
     * @param   array   $neolines   書き込むデータの配列
     * @return  void
     * @access  private
     * @static
     */
    function _writeData($path, $neolines)
    {
        if (is_array($neolines) && count($neolines) > 0) {
            $cont = implode('', $neolines);
        /*} elseif (is_scalar($neolines)) {
            $cont = strval($neolines);*/
        } else {
            $cont = '';
        }
        if (FileCtl::file_write_contents($path, $cont) === false) {
            $errmsg = sprintf('Error: cannot write file. (%s)', htmlspecialchars($path, ENT_QUOTES));
            die($errmsg);
        }
    }

    // }}}
    // {{{ _detectHostType()

    /**
     * ホストの種類を判定する
     *
     * @param   string  $host   ホスト名
     * @return  string  ホストの種類
     * @access  private
     * @static
     */
    function _detectHostType($host)
    {
        if (P2Util::isHostBbsPink($host)) {
            $type = 'bbspink';
        } elseif (P2Util::isHost2chs($host)) {
            $type = '2channel';
        } elseif (P2Util::isHostMachiBbs($host)) {
            $type = 'machibbs';
        } elseif (P2Util::isHostJbbsShitaraba($host)) {
            $type = 'jbbs';
        } else {
            $type = $host;
        }
        return $type;
    }

    // }}}
}

// }}}
?>
