/***********************************************************
 *  mj script for html mahjiong game.
 *  by Terry.Yinzhe@gmail.com
 * 
 *
 */
var converter = new Array();
converter[0]=0;converter[49]=1;converter[50]=2;converter[51]=3;converter[52]=4;converter[53]=5;converter[54]=6;converter[55]=7;converter[56]=8;converter[57]=9;converter[65]=21;converter[66]=22;converter[67]=23;converter[68]=24;converter[69]=25;converter[70]=26;converter[71]=27;converter[72]=28;converter[73]=29;converter[97]=41;converter[98]=42;converter[99]=43;converter[100]=44;converter[101]=45;converter[102]=46;converter[103]=47;converter[104]=48;converter[105]=49;converter[108]=61;converter[111]=64;converter[114]=67;converter[117]=70;converter[120]=73;converter[123]=76;converter[126]=79;
var App = {
		max_holding_count : 13,
		max_eaten_count : 4,
		player_count : 2,
		current_player : 0,
		holdings : Array(this.player_count),
		current : Array(this.player_count),
		eaten : Array(this.player_count),
		board_index : 1,
		chowing : false,
        setup : function () 
        {
        		for (j = 0; j < this.player_count; j++) {
        			this.holdings[j] = Array(0);
        			this.eaten[j] = Array(0);
        		}
        			
                document.onclick = App.onclick;
                
				this._AddNotification();
				this.StartGame();
                this._ExecuteCmd("start", 0);   
        },
        StartGame : function () {
				this._SetupPage();        	
        },
        UpdateHolding : function (new_holdings) {
        	for(var j = 0; j < this.player_count; j++) {
        		var cnt = 0;
        		for (i in new_holdings[j])
        			if (new_holdings[j][i] < 0x80)
        				cnt++;
        			else
        				break;
        		this.holdings[j] = Array(cnt - 1)
        		for (var i = 0; i < cnt -1; i++)
        			this.holdings[j][i] = new_holdings[j][i];
        		this.current[j] = new_holdings[j][i];
        		this.eaten[j] = Array(new_holdings[j].length - cnt)
        		for (cnt=0, i++; i < new_holdings[j].length; i++) {
        			this.eaten[j][cnt++] = new_holdings[j][i];
        		}
        	}
            this._UpdateAllCells();
        },
        Pick : function (player, new_tile) {
            if (new_tile == 0) {
        		alert("Nobody wins.");
                this._SetupBoard();
                this._UpdateAllCells();
                this._ExecuteCmd("start", 0);   
                this._ExecuteCmd("all", 0);   
            }
            else {
        		this.current[player] = new_tile;
            	this._UpdateAllCells();
            }
        },
        _resetChowing : function () {
        	this._showStatus("action","");
        	this.chowing = false;
        },
        Chow : function () {
        	if (this.chowing)
        		this._resetChowing();
			else if (App._isPicking()) {
        		this._showStatus("action","Please click on the smallest tile you want to chow with.");
        		this.chowing = true;
			}
        },
        Chow_with_tile : function (tile) {
        	this._ExecuteCmd('chow', tile);
        },
        Pong : function () {
        	this._ExecuteCmd('pong', 0);
        },
        Kong : function () {
        	this._ExecuteCmd('kong', 0);
        },
        Win : function () {
        	this._ExecuteCmd('win', 0);
        },
        WinAck : function (reserved, score) {
        	this._ResetAllButtons();
        	this.StopUpdate();
			if (score == 0) {
        		alert("Nobody wins.");
            }
            else {
        		alert("You win ("+score+")!");
            }
            this._SetupBoard();
            this._UpdateAllCells();
            this._ExecuteCmd("start", 0);   

        },
		onclick : function(e) {
			try {
				target = App._findClickableTarget(e.target);
			}
			catch (ex){
				target = App._findClickableTarget(window.event.srcElement);
			}

			if (target) {
				if (App._isPicking() && !App.chowing) {
					App._Pick();
				}
				else {
					discard = -1
					id = target.id.substr(5);
					if (id == 'current') {
						discard = App.current[0];
					}
					else {
						index = parseInt(id);
						index = App._CellIndexToHoldingIndex(0, index);
						if (index >= 0) {
							discard = App.holdings[0][index];
						}
					}
					if (discard != -1)
						if (App.chowing)
							App.Chow_with_tile(discard);
						else
							App._Throw(discard);
				}
			}
		},
		_isPicking : function () {
			if (this.current[0]==0) {
				if ((this.holdings[0].length % 3)==1)
					return true;
			}
			return false;
		},
		_Throw : function (tile) {
			this._ResetAllButtons();
			App._ExecuteCmd('throw', tile);
		},
		_Pick : function () {
			this._ResetAllButtons();
			App._ExecuteCmd('pick', 0);
		},
		Throw : function (tile, player) {
			if (tile != 0) {
				cell = document.getElementById("sh_"+App.board_index);
				App.board_index++;
				cell.innerHTML = "<img src='images/"+converter[tile]+".png' height=64 width=41 style='-webkit-transform: rotate(180deg);-moz-transform: rotate(180deg);filter: progid:DXImageTransform.Microsoft.BasicImage(rotation=2);'>"
			}
			this._Picking(player);
		},
		RemoveLast : function () {
			cell = document.getElementById("sh_"+(App.board_index-1));
			cell.className = "eaten";
		},
		LightButton : function (button) {
			cell = document.getElementById(button+"_button");
			cell.style.cssText = "background: #FF0000;"
		},
		_ResetAllButtons : function () {
			this._ResetButton("win")
			this._ResetButton("chow")
			this._ResetButton("kong")
			this._ResetButton("pong")
		},
		_ResetButton : function (button) {
			cell = document.getElementById(button+"_button");
			cell.style.cssText = ""
		},
		t:-1,
		
		ResumeUpdate: function () {
			if (this.t==-1) {
				this.t = setInterval("App._Update()", 250);
			}
		},
		StopUpdate : function () {
			if (this.t!=-1) {
				clearInterval(this.t);
				this.t=-1;
			}
		},
		_Update : function () {
			App._ExecuteCmd('update', 0)
		},
		_Picking : function (player) {
			this.current_player = player;
		},
		_findClickableTarget: function( target )
		{
		    // If a table cell, we assume that's editable
		    try {
			    if (target.id.substr(0,4) == "ph_0")
			        return target;
			
			    if( target.parentNode )
			        return this._findClickableTarget( target.parentNode );
			}
			catch (e) {}		
		    return null;
		}, 
        _ExecuteCmd : function (cmd, param) {
        	this._resetChowing();
        	try {
        		http = new XMLHttpRequest();
        	}
        	catch (e) {
        		http=new ActiveXObject("Microsoft.XMLHTTP");
        	}
        	param = game_id * 1000 + param;
        	http.open("GET", cmd + '?' + param, true);
        	http.send(null);
			http.onreadystatechange = function () {
			  if (http.readyState == 4) {
    			  var textout = http.responseText;
    			  eval(textout);
    		  }
			};
        },
        
        onexit : function ()
        {
             //return ""
                
        },

        _showStatus:function(desc, result)
        {
                tstat = document.getElementById("tabel_status")
                if (tstat.innerText)
                        tstat.innerText = desc+":"+result
                else
                        tstat.textContent = desc+":"+result
                
        },
        _AddNotification: function ()
        {
                newNode=document.createElement("span")
                newNode.setAttribute("id","tabel_status");
                newNode.style.cssText = "position: absolute;"+
                                        "margin: 0px;"+
                                        "padding: 0px;"+
                                        "font-family: verdana;"+
                                        "font-size: 12px;"+
                                        "background: #FFAA88;"+
                                        "left:0px;top:0px;"
                newNode.innerHTML = "Mahjiong game is working! RNCIPA Coach Network"
                document.body.appendChild(newNode);
        },
        _SetupPage : function()
        {
                newNode=document.createElement("span")
                newNode.setAttribute("id","game_board");
                newNode.innerHTML = "<table>\n"+
                						"<tr><td id=player_1> put me here"+
										"<tr><td id=board0>\n"+
										"<tr><td id=player_0>\n"+
										"<tr><td id=panel0></table><div id='log'>log...</div>"
                document.body.appendChild(newNode);
                this._SetupPlayers();
                this._SetupBoard();
                this._SetupPannel();
        },
        _SetupPlayers : function() {
        	for (var i = 0; i < this.player_count; i++) {
        		this._SetupPlayer(i);
        	}
        },
        _SetupPlayer : function(player) {
        	html = "<table class=player><tr><td width=50><table class='eaten_plate'><tr>"
        	for (i = 0; i < this.max_eaten_count; i++)
        		html += ("<td id=eaten_"+player+"_"+i+">");
        	html = html+"</table><td><table><tr>"
            cnt = 1;
            for (var i=0; i<this.max_holding_count; i++) {
            		html+=("<td id=ph_" + player+"_"+i+">");
            }
            html += ("<td>-<td id=ph_"+player+"_current></table></table>");
            player_html = document.getElementById("player_"+ player);
            player_html.innerHTML = html;
        },

        _SetupBoard : function() {
        	this.board_index = 1;
            html = "<table><tr><td width=800 height=150><table>"
            cnt = 1;
            for (i=0; i<4; i++) {
            	html+="<tr>"
            	for (j=0; j < 15; j++) {
            		html+=("<td id=sh_" + cnt+">");
            		cnt++;
            	}
            } 
            board = document.getElementById("board0");

            board.innerHTML = html;
        },
        _SetupPannel : function() {
           
            panel = document.getElementById("panel0");
            panel.innerHTML = "<table><tr>"+
            	"<td> <a class='button' id=chow_button onclick='return App.Chow()'>Chow</a>"+
            	"<td> <a class='button' id=pong_button onclick='return App.Pong()'>Pong</a>"+
            	"<td> <a class='button' id=kong_button onclick='return App.Kong()'>Kong</a>"+
            	"<td> <a class='button' id=win_button onclick='return App.Win()'>Win!!</a>"+
            	"</table>";
            
        },
        _UpdateAllCells : function() {
        	for (j = 0; j < this.player_count; j++) {
        		for (i =0; i < this.max_eaten_count; i++) {
        			this._UpdateEaten(j, i)
        		}
            	for (i = 0; i <=this.max_holding_count; i++)
            		this._UpdateCell(j, i);
        	}
        },
        _UpdateEaten: function (player, index) {
          	cell = document.getElementById("eaten_"+player+"_"+index);
          	if (index < this.eaten[player].length) {
          		inc = 0
          		e = this.eaten[player][index];
          		if (e > 0x100) {
          			e -= 0x100;
          			inc = 1;
          		}
          		else
          			e-= 0x80;
          		e= converter[e];
          		html = "<table class=eaten_w><tr>"
          		html += ("<td class=eaten_w>"+"<img class=eaten_plate src='images/"+e+".png' height=64 width=41>");
          		e += inc;
          		html += ("<td class=eaten_w>"+"<img class=eaten_plate src='images/"+e+".png' height=64 width=41>");
          		e += inc;
          		html += ("<td class=eaten_w>"+"<img class=eaten_plate src='images/"+e+".png' height=64 width=41>");
          		html += "</table>"
          		cell.innerHTML = html;
          	}
        	else {
        		cell.innerHTML = "";
        	}
        	
        },
        _CellIndexToHoldingIndex : function(player, cell_index) {
        	diff = this.max_holding_count - this.holdings[player].length;
        	return cell_index >= diff ? cell_index - diff: -1;
        },
        _UpdateCell : function(player, index) {
           if (index != this.max_holding_count) {
          		cell = document.getElementById("ph_"+player+"_"+index);
           		holding_index = this._CellIndexToHoldingIndex(player, index);
           		if (holding_index >=0)
           			tile = this.holdings[player][holding_index];
           		else
           			tile = 0;
           }
           else {
           		cell = document.getElementById("ph_"+player+"_current");
           		tile = this.current[player];
           }
           if (tile == 0 && index != this.max_holding_count) 
           		cell.innerHTML = "";
           else
           		cell.innerHTML = "<img class='active_tile' src='images/"+converter[tile]+".png' height=64 width=41>"
        	
        },
        
        the_end : 1
}

function setup()
{
	App.setup();
}
window.onload= setup;
window.onbeforeunload = App.onexit;


