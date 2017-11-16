/*Explanation
1.include js and css
<link rel="stylesheet" type="text/css" href="/js/table/table.css">
<script type="text/javascript" language="JavaScript" src="/js/table/table.js"></script>


2. data setting
var rawData = []; Necessary, data array, parse from nvram
ex:
var rawData = [
	[value1, value2,...],
	....
];

3. table struct 
var tableStruct = {
	// data block
	data: link from rawData
	ex: data: rawData

	// container ID block
	container: Necessary, The id from asp div
	ex: container : tableContainer

	// table title block
	title: Optional, table title description
	ex: title: "Server List",

	// capability block
	capability : // control table capability
	{
		add: Optional, Whether to allow create rule function, value is true / flase
		del: Optional, whether to allow delete the rule, value is true/false
		clickEdit: Optional, data raw click will change edit mode, value is true/false
		hover: Optional, data raw hover will change color, value is true/false
	},

	// header block
	header : Necessary, table header item json array
	The json have there item as below
	{
		title : Necessary, title text
		width : Optional, defalut is average
		sort : Optional, sort type, value is str/num, the default define at tableSorter
		defaultSort : Optional, default sort index, this attribure is only, if not define will find fist define sort item
	}
	ex: 
	header: [ 
		{
			"title" : "Trigger Port",
			"sort" : "num",
			"width" : "40%"
		},
		{
			"title" : "Protocol",
			"sort" : "str",
			"width" : "40%"
		},
		{
			"title" : "<#CTL_del#>",
			"width" : "10%"
		}
	],

	// create new rule block
	createPanel : Optional, Create new rule item json array
	createPanel : {
		inputs : [], each item json array
		maximum: Optional, The number of table allowed to be added, defalut is 16
	}
	The inputs json have there item as below
	1.	text :
		editMode : Necessary, edit type is text
		title : Necessary, title text
		maxlength : Optional, the input max length, defalut is not limit
		valueMust : Optional, whether to allow the input is null, default is true, value is true/flase
		validator : Optional, the function define at tableValidator.js, the value is function name
		placeholder : Optional, define placeholder hint
	2.	select :
		editMode : Necessary, edit type is select
		title : Necessary, title text
		option : Necessary, the select option, value format is {"text" : "value", ... }
	ex: 
	createPanel: {
		inputs : [
			{
				"editMode" : "text",
				"title" : "Trigger Port",
				"maxlength" : "5",
				"validator" : "portRange"
			},
			{
				"editMode" : "text",
				"title" : "Description",
				"maxlength" : "18",
				"valueMust" : false,
				"validator" : "description"
			},
			{
				"editMode" : "select",
				"title" : Protocol,
				"option" : {"TCP" : "TCP", "UDP" : "UDP"}
			}
		],
		maximum: 32
	}
	
	// click raw edit rule block
	clickRawEditPanel : Optional, edit rule item json array
	clickRawEditPanel : {
		inputs : [], each item json array
	}
	The json have there item as below
	1.text : edit input
		editMode : Necessary, edit type is text
		maxlength : Optional, the input max length, defalut is not limit
		validator : Optional, the function define at tableValidator.js, the value is function name
	2.select : edit select
		editMode : Necessary, edit type is select
		option : Necessary, the select option, value format is {"text" : "value", ... }
	3.callBack : call back function
		editMode : Necessary, edit type is callBack
		callBackFun : Call back function name
	4.pureText : only change edit mode
		editMode : Necessary, edit type is pureText
	Common attribute :
		className : Assign specific class, ex. PingStatus
		styleList : Assign specific style list,  {"cursor":"pointer", "font-size":"20px"}
	ex: 
	clickRawEditPanel : {
		inputs : [
			{
				"editMode" : "text",
				"maxlength" : "18",
				"validator" : "description",
				"className" : "PingStatus",
				"styleList" : {"cursor":"pointer"}
			},
			{
				"editMode" : "text",
				"maxlength" : "5",
				"validator" : "portRange",
				"className" : "PingStatus",
				"styleList" : {"cursor":"pointer"}
			},
			{
				"editMode" : "select",
				"option" : {"TCP" : "TCP", "UDP" : "UDP"},
				"className" : "PingStatus",
				"styleList" : {"cursor":"pointer", "font-size":"20px"}
			},
			{
				"editMode" : "callBack",
				"callBackFun" : Call back function name,
				"className" : "PingStatus",
				"styleList" : {"cursor":"pointer", "font-size":"20px"}
			},
			{
				"editMode" : "pureText",
				"className" : "PingStatus",
				"styleList" : {"cursor":"pointer", "font-size":"20px"}
			}
		]
	}

	ruleDuplicateValidation : Valid rule data wherther duplicate or not, the value is function name from tableValidator.js tableRuleDuplicateValidation function
	ruleValidation : Valid rule data special rule, the value is function name from tableValidator.js tableRuleValidation function
}

4. Call table API
tableApi.genTableAPI(tableStruct);
*/

document.write('<script type="text/javascript" src="/js/table/tableValidator.js"></script>');
var tableSorter = {
	"sortFlag" : false,
	"indexFlag" : 0, // defalut sort index is 0
	"sortingType" : "str", // defalut sort type is str
	"sortingMethod" : "increase", // defalut sort type is increase,  value is increase/decrease

	"num_increase" : function(a, b) {
		return parseInt(a[tableSorter.indexFlag]) - parseInt(b[tableSorter.indexFlag]);
	},
	"num_decrease" : function(a, b) {
		return parseInt(b[tableSorter.indexFlag]) - parseInt(a[tableSorter.indexFlag]);
	},
	"str_increase" : function(a, b) {
		if(a[tableSorter.indexFlag].toString().toUpperCase() == b[tableSorter.indexFlag].toString().toUpperCase()) return 0;
		else if(a[tableSorter.indexFlag].toString().toUpperCase() > b[tableSorter.indexFlag].toString().toUpperCase()) return 1;
		else return -1;
	},
	"str_decrease" : function(a, b) {
		if(a[tableSorter.indexFlag].toString().toUpperCase() == b[tableSorter.indexFlag].toString().toUpperCase()) return 0;
		else if(a[tableSorter.indexFlag].toString().toUpperCase() > b[tableSorter.indexFlag].toString().toUpperCase()) return -1;
		else return 1;
	},
	"drawBorder" : function(_clickIndex) {
		var clickSortingMethod = tableSorter.sortingMethod;
		var boxShadowTopCss = "inset 0px -1px 0px 0px #FC0";
		var boxShadowBottomCss = "inset 0px 1px 0px 0px #FC0";

		if(clickSortingMethod == "increase") {
			$(".row_title").children().eq(_clickIndex).css("box-shadow", boxShadowTopCss);
		}
		else {
			$(".row_title").children().eq(_clickIndex).css("box-shadow", boxShadowBottomCss);
		}
	},
	
	"sortData" : function(_flag, _Method, _dataArray) {
		eval("" + _dataArray + ".sort(tableSorter." + _Method + "_" + tableSorter.sortingMethod + ");");
	},

	"doSorter" : function(_$this) {	
		// update variables
		tableSorter.indexFlag = _$this.index();
		tableSorter.sortingMethod = (tableSorter.sortingMethod == "increase") ? "decrease" : "increase";
		tableSorter.sortingType = _$this.attr("sortType");
		tableSorter.sortData(tableSorter.indexFlag, tableSorter.sortingType, "tableApi._jsonObj.data");
		tableApi.genTable(tableApi._jsonObj);
		tableSorter.drawBorder(tableSorter.indexFlag);
	}
}
var tableApi = {
	_jsonObj : "",

	_attr : {
		// data block
		"data" : "", // date item array

		// container ID block
		"container" : "", // parent div id
		
		// table title block
		"title" : "", // table title

		// capability block
		"capability": {
			add: false, //create new rule flag, true/false
			del: false, // date raw delete
			clickEdit: false, // date raw click flag, true/false
			hover: false //date item hover events
		},

		// head block
		"header" : "", // header item array
		
		// create new rule block
		"createPanel" : {
			inputs : "", //create new rule item
			maximum: 16 //create new rule count
		},

		// click raw edit rule block
		"createPanel" : {
			inputs : "" //edit rule item
		},
		
		"ruleDuplicateValidation" : "", // Valid rule data wherther duplicate or not
		"ruleValidation" : "", // Valid rule data special rule
	},

	_privateAttr : {
		"header_item_num" : 0, // header item count
		"header_item_width" : "", // header item width
		"data_num" : 0, // date item count
	},


	genTableAPI : function (json_struct) {
		tableApi._jsonObj = json_struct;

		if (tableApi._jsonObj.hasOwnProperty("header")) {
			var headItemTmp = tableApi._jsonObj.header.slice();
			for (var opt in headItemTmp) {
				if (headItemTmp.hasOwnProperty(opt)) {
					if(headItemTmp[opt].hasOwnProperty("sort")) {
						if(!tableSorter.sortFlag) {
							//find first have sort attribute
							tableSorter.sortingType = headItemTmp[opt]["sort"];
							tableSorter.indexFlag = opt;
							tableSorter.sortFlag = true;
						}
						
						if(headItemTmp[opt].hasOwnProperty("defaultSort")) {
							//if define defaultSort, update value
							tableSorter.sortingMethod = headItemTmp[opt]["defaultSort"];
							tableSorter.sortingType = headItemTmp[opt]["sort"];
							tableSorter.indexFlag = opt;
							break;
						}
					}
				}
			}
			
		}

		if(tableSorter.sortFlag)
			tableSorter.sortData(tableSorter.indexFlag, tableSorter.sortingType, "tableApi._jsonObj.data");

		tableApi.genTable(tableApi._jsonObj);

		if(tableSorter.sortFlag)
			tableSorter.drawBorder(tableSorter.indexFlag);
	},

	init : function(json_struct) {
		var obj = json_struct;
		for (var opt in obj) {
			if (obj.hasOwnProperty(opt)) {
				if(opt == "capability" || opt == "createPanel") {
					for (var subOpt in obj[opt]) {
						if (obj[opt].hasOwnProperty(subOpt)) {
							tableApi._attr[opt][subOpt] = obj[opt][subOpt];
						}
					}
				}
				else {
					tableApi._attr[opt] = obj[opt];
				}

				switch(opt) {
					case "header" :
						tableApi._privateAttr.header_item_num = tableApi._attr[opt].length;
						break;
					case "data" :
						tableApi._privateAttr.data_num = tableApi._attr[opt].length;
						break;
				}
			}
		}

		tableApi._privateAttr.header_item_width = new Array();
		var table_width = (tableApi._attr.capability.del) ? 90 : 100;
		var width = (table_width / tableApi._privateAttr.header_item_num) + "%";
		for(var i = 0; i < tableApi._privateAttr.header_item_num; i += 1) {
			if(tableApi._attr.header[i].hasOwnProperty("width"))
				tableApi._privateAttr.header_item_width.push(tableApi._attr.header[i].width);
			else
				tableApi._privateAttr.header_item_width.push(width);
		}
		if(tableApi._attr.capability.del) {
			tableApi._privateAttr.header_item_num++;
		}
		
		if($("#" + tableApi._attr.container).html().length > 0) {
			$("#" + tableApi._attr.container).empty()
		}
	},

	//Create new Rule start
	genAddRule_frame : function(_createRule, _createRuleItem, _createRuleCount, _dataCount, _tableName) {
		var $addRuleHtml = "";
		if(_createRule && _createRuleItem.length != 0) {
			$addRuleHtml = $("<div>");
			$addRuleHtml.addClass("addRuleFrame");
			var _tableNameText = (_tableName == "") ? "Rule List" : _tableName;
			var $addTextHtml = $("<div>").html("" + _tableNameText + " ( <#List_limit#> " + _createRuleCount + " )").addClass("addRuleText");
			$addTextHtml.appendTo($addRuleHtml);

			var $addIconHtml = $("<div>").addClass("addRule");
			$addIconHtml.appendTo($addRuleHtml);
			$addIconHtml.click(
				function() {
					if($(".row_tr").children().find(".hint").length != 0) {
						return false;
					}
					if(_dataCount >= _createRuleCount){
						alert("<#JS_itemlimit1#> " + _createRuleCount + " <#JS_itemlimit2#>");
						return;
					}
					$('body').prepend(tableApi.genCreateRule(_createRuleItem));
					$('body').prepend(tableApi.genFullScreen());

					$('.fullScreen').fadeIn();
					$('.createNewRule').fadeIn();

					var scrollTop = $(document).scrollTop();
					$(".createNewRule").css({ top: (scrollTop + 200) + "px" });
					//$(".createNewRule").children().find(".inputText").first().focus();
				}
			);
		}
		return $addRuleHtml;
	},

	genFullScreen : function() {
		var $fullScreenHtml = $("<div>");
		$fullScreenHtml.addClass("fullScreen");
		$fullScreenHtml.attr({"onselectstart" : "return false"});
		return $fullScreenHtml;
	},

	genCreateRule : function(_createRuleItem) {
		tableApi.removeElement("fullScreen");
		tableApi.removeElement("createNewRule");

		var $divHtml = "";
		$divHtml = $("<div>");
		$divHtml.addClass("createNewRule");

		//title
		var $titleHtml = $("<div>");
		$titleHtml.addClass("pureText");
		$titleHtml.html("Create New Rule");
		$titleHtml.appendTo($divHtml);

		//close icon
		var $closeHtml = $("<div>");
		$closeHtml.addClass("closeIcon");
		$closeHtml.click( tableApi.closeRuleFrame );
		$closeHtml.appendTo($divHtml);

		$divHtml.append(tableApi.genDivisionLine());

		for (var idx in _createRuleItem) {
			var newItemObj =  _createRuleItem[idx];
			if(newItemObj.hasOwnProperty("editMode")) {
				var $newItemframe = $("<div>");

				var $newItemTitleHtml = "";
				$newItemTitleHtml = $("<div>");
				$newItemTitleHtml.addClass("pureText");
				$newItemTitleHtml.html(newItemObj.title);
				$newItemTitleHtml.appendTo($newItemframe);

				var $newItemEditHtml = "";
				switch(newItemObj.editMode) {
					case "text" :
						$newItemEditHtml = tableApi.genCreateRuleText(newItemObj);
						break;
					case "select" :
						$newItemEditHtml = tableApi.genCreateRuleSelect(newItemObj);
						break;
					case "textAndClientList" :
						$newItemEditHtml = tableApi.genCreateRuleTextAndClientList(newItemObj);
						break;
				}
				$newItemEditHtml.appendTo($newItemframe);
			}

			$newItemframe.appendTo($divHtml);
		}

		$divHtml.append(tableApi.genDivisionLine());

		//action control
		var $actionFrameHtml = $("<div>");

		var $actionTextHtml = $("<div>");
		$actionTextHtml.addClass("actionText");
		$actionTextHtml.html("Editoring " + (tableApi._privateAttr.data_num + 1)  + " Rule");
		$actionTextHtml.appendTo($actionFrameHtml);

		var $actionButtonFrameHtml = $("<div>");
		$actionButtonFrameHtml.addClass("actionButtonFrame");

		var $actionButtonCancelHtml = $("<div>");
		$actionButtonCancelHtml.addClass("actionButtonCancel");
		$actionButtonCancelHtml.html("<#CTL_Cancel#>");
		$actionButtonCancelHtml.click( tableApi.closeRuleFrame );
		$actionButtonCancelHtml.appendTo($actionButtonFrameHtml);

		var $actionButtonOKHtml = $("<div>");
		$actionButtonOKHtml.addClass("actionButtonOK");
		$actionButtonOKHtml.html("<#CTL_ok#>");
		$actionButtonOKHtml.click(
			function() {
				var newRuleArray = new Array();
				$(".saveRule").each(function() {
					if($(this).hasClass("valueMust")) {
						if($(this).val().trim() == "") {
							$(this).blur();
							$(this).focus();
							return false;
						}
					}
					newRuleArray.push($(this).val());
				});

				
				if($(".createNewRule").children().find(".hint").length == 0) {
					var validDuplicateFlag = true;
					if(tableApi._attr.hasOwnProperty("ruleDuplicateValidation") && (tableApi._attr.ruleDuplicateValidation != "")) {
						validDuplicateFlag = tableRuleDuplicateValidation[tableApi._attr.ruleDuplicateValidation](newRuleArray, tableApi._attr.data);
						if(!validDuplicateFlag) {
							alert("<#JS_duplicate#>");
							return false;
						}
					}

					var ruleValidationFlag = true;
					if(tableApi._attr.hasOwnProperty("ruleValidation") && (tableApi._attr.ruleValidation != "")) {
						ruleValidationFlag = tableRuleValidation[tableApi._attr.ruleValidation](newRuleArray);
						if(ruleValidationFlag != HINTPASS) {
							alert(ruleValidationFlag);
							return false;
						}
					}
					
					if(validDuplicateFlag && ruleValidationFlag) {
						tableApi._attr.data.push(newRuleArray);
						if(tableSorter.sortFlag)
							tableSorter.sortData(tableSorter.indexFlag, tableSorter.sortingType, "tableApi._jsonObj.data");

						tableApi.genTable(tableApi._attr);

						if(tableSorter.sortFlag)
							tableSorter.drawBorder(tableSorter.indexFlag);
						tableApi.closeRuleFrame();
					}
				}
			}
		);
		$actionButtonOKHtml.appendTo($actionButtonFrameHtml);

		$actionButtonFrameHtml.appendTo($actionFrameHtml);

		$divHtml.append($actionFrameHtml);
			
		return $divHtml;
	},

	closeRuleFrame : function() {
		$("body").find(".fullScreen").fadeOut();
		$("body").find(".createNewRule").fadeOut();
		setTimeout( 
			function() {
				tableApi.removeElement("createNewRule");
				tableApi.removeElement("fullScreen");
			}, 1000
		);
	},

	genCreateRuleText : function(_itemObj) {
		var $newItemHtml = "";
		$newItemHtml = $('<input/>');
		$newItemHtml.addClass("saveRule valueMust inputText");
		$newItemHtml.attr({"type" : "text", "autocorrect" : "off", "autocapitalize" : "off"});
		if(_itemObj.hasOwnProperty("maxlength")) {
			$newItemHtml.attr({"maxlength" : _itemObj.maxlength});
		}
		if(_itemObj.hasOwnProperty("valueMust")) {
			$newItemHtml.removeClass("valueMust");
		}

		if(_itemObj.hasOwnProperty("placeholder")) {
			$newItemHtml.attr({"placeholder" : _itemObj.placeholder});
		}

		if(_itemObj.hasOwnProperty("validator")) {
			if(tableValidator[_itemObj.validator]["keyPress"]) {
				$newItemHtml.keypress(
					 function() {
						return tableValidator[_itemObj.validator]["keyPress"]($(this), event);
					}
				);
			}
			if(tableValidator[_itemObj.validator]["blur"]) {
				$newItemHtml.blur(
					 function() {
					 	tableValidator[_itemObj.validator]["blur"]($(this));
					}
				);
			}
		}

		return $newItemHtml;
	},

	genCreateRuleSelect : function(_itemObj) {
		var $newItemHtml = "";
		$newItemHtml = $('<select/>');
		$newItemHtml.addClass("saveRule inputSelect");
		if(_itemObj.hasOwnProperty("option")) {
			for (var optIdx in _itemObj.option) {
				var optionText = optIdx;
				var optionValue =_itemObj.option[optIdx];
				var $optionHtml = $('<option/>');
				$optionHtml.attr("value", optionValue).text(optionText);
				$newItemHtml.append($optionHtml);
			}
		}
		return $newItemHtml;
	},

	genCreateRuleTextAndClientList : function(_itemObj) {
		var $newItemFrameHtml = $('<div>');
		$newItemFrameHtml.addClass("editFrame");
		var $newItemHtml = tableApi.genCreateRuleText(_itemObj);
		

		$newItemHtml.appendTo($newItemFrameHtml);

		var $newPullArrowHtml = $("<img>");
		$newPullArrowHtml.addClass("pullArrow");
		$newPullArrowHtml.attr({
			src: "/images/arrow-down.gif",
			title: "jQuery",
			alt: "jQuery Logo"
		});


		$newPullArrowHtml.appendTo($newItemFrameHtml);

		return $newItemFrameHtml;
	},
	//Create new Rule end

	
	genTable: function(json_struct){

		tableApi.init(json_struct);
		
		//Add new Rule
		$("#" + tableApi._attr.container).append(
			tableApi.genAddRule_frame(tableApi._attr.capability.add, tableApi._attr.createPanel.inputs, tableApi._attr.createPanel.maximum, tableApi._privateAttr.data_num, tableApi._attr.title)
		);

		//table main
		$("#" + tableApi._attr.container)
			//table
			.append(
				tableApi.genTable_frame(tableApi._attr)
					// thead
					//.append(
					//	tableApi.genThead_frame(tableApi._attr.title, tableApi._privateAttr.header_item_num)
					//)
					// title
					.append(
						tableApi.genTitle_frame(tableApi._attr.header, tableApi._privateAttr.header_item_width)
					)
					// content
					.append(
						tableApi.genContent_frame(tableApi._attr, tableApi._privateAttr.header_item_width, tableApi._privateAttr.data_num)
					)
			)

		//table hover evenr
		tableApi.setHover($("#" + tableApi._attr.container), tableApi._attr.capability.hover);

		tableApi.setDataRawClickEvents(tableApi._attr.capability.clickEdit, tableApi._attr.clickRawEditPanel.inputs, tableApi._attr.data);
		
		tableApi.genDataRawDel($("#" + tableApi._attr.container));

		tableApi.setHeaderSort(tableApi._attr.header, tableSorter.sortFlag);
	},

	setHeaderSort : function(_headerObj, _sortFlag) {
		if(_sortFlag) {
			//$(".row_title").children().css("border-bottom", "1px solid #222");
			//$(".row_title").children().css("border-top", "1px solid #222");
			for(var i in _headerObj){
				if (_headerObj.hasOwnProperty(i)) {
					if (_headerObj[i].hasOwnProperty("sort")) {
						$(".row_title").children().eq(i).css({"cursor" : "pointer"});
						$(".row_title").children().eq(i).attr({"sortType" : _headerObj[i].sort});
						$(".row_title").children().eq(i).click(
							function() {
								if($(".row_tr").children().find(".hint").length != 0) {
									return false;
								}
								tableSorter.doSorter($(this));
							}
						);
					}
				}
			}
		}	
	},

	genTable_frame : function() {
		var $tableHtml = $("<table>");
		$tableHtml
			.attr("cellpadding", "4")
			.addClass("tableApi_table");
		return $tableHtml;
	},

	genThead_frame : function(_headName, _itemNum) {
		var $headHtml = "";
		if(_headName != "") {
			$headHtml = $("<thead>");
			$("<td>")
				.attr("colspan",_itemNum)
				.html(_headName)
				.appendTo($headHtml);
		}
		return $headHtml;
	},

	genTitle_frame : function(_headerObj, _headerWidthArray) {
		var $titleHtml = $("<tr>");
		$titleHtml.addClass("row_title");
		for(var i in _headerObj){
			if (_headerObj.hasOwnProperty(i)) {
				$("<th>")
				.attr("width", _headerWidthArray[i])
				.html(_headerObj[i].title)
				.appendTo($titleHtml);
			}
		}
		return $titleHtml;
	},

	genContent_frame : function(_obj_attr, _headerWidthArray, _dataNum) {
		var contentArray = [];
		if(_dataNum != 0) {
			for(var j = 0; j < _dataNum; j += 1) {
				var currRow = tableApi._attr.data[j];
				var $contentHtml = $("<tr>");
				$contentHtml.addClass("row_tr");
				$contentHtml.addClass("data_tr");
				$contentHtml.attr("row_tr_idx", j);
				
				for(var k in currRow){
					if (currRow.hasOwnProperty(k)) {
						var _dataItemText = currRow[k];
						if(_obj_attr.clickRawEditPanel.inputs != "") {
							if(_obj_attr.clickRawEditPanel.inputs[k] != undefined) {
								if(_obj_attr.clickRawEditPanel.inputs[k].editMode == "select") {
									_dataItemText = tableApi.transformSelectOptionToText(_obj_attr.clickRawEditPanel.inputs[k].option, _dataItemText);
								}
							}
						}
						var dataRawClass = "";
						var dataRawStyleList = "";
						if (_obj_attr.clickRawEditPanel.inputs[k].hasOwnProperty("className")) {
							dataRawClass = _obj_attr.clickRawEditPanel.inputs[k].className;
						}
						if (_obj_attr.clickRawEditPanel.inputs[k].hasOwnProperty("styleList")) {
							dataRawStyleList = _obj_attr.clickRawEditPanel.inputs[k].styleList;
						}
						var dataRawStyleListJson = {};
						Object.keys(dataRawStyleList).forEach(function(key) {
							if (dataRawStyleList.hasOwnProperty(key)) {
								dataRawStyleListJson[key] =dataRawStyleList[key];
							}
						});
						$("<td>")
							.addClass("row_td")
							.css(dataRawStyleListJson)
							.addClass(dataRawClass)
							.attr("row_td_idx", k)
							.attr("width", _headerWidthArray[k])
							.html(
								$("<div>")
									.addClass("static-text")
									.attr("width", "100%")
									.html(_dataItemText)
							)
							.appendTo($contentHtml);
					}	
				}
				contentArray.push($contentHtml);
			}
		}
		else {
			var $contentHtml = $("<tr>");
			$contentHtml.addClass("data_tr");
			$("<td>")
				.attr("colspan", tableApi._privateAttr.header_item_num)
				.css("color", "#FC0")
				.html("<#IPConnection_VSList_Norule#>")
				.appendTo($contentHtml);
			contentArray.push($contentHtml);
		}
		return contentArray;
	},

	setDataRawClickEvents : function(_clickFlag, _dataItemObj, _dataArray) {
		if(_clickFlag && _dataItemObj.length != 0) {
			tableApi.registerBodyEvent();
			//gen data raw ietm edit element
			tableApi.genDataRawEditElement(_dataItemObj, _dataArray);
		}
	},

	registerBodyEvent : function() {
		$('body').click(
			function() {
				if($(".row_tr").children().find(".hint").length != 0) {
					return false;
				}
				if($('body').find('.row_tr').hasClass("data_raw_editing")) {	
					$('body').find('.row_tr').removeClass("data_raw_editing");
					$('body').find('.row_tr').find('.edit-mode').css({"display" : 'none'});
					$('body').find('.row_tr').find('.static-text').css({"display" : ''});
					if(tableSorter.sortFlag)
						tableSorter.sortData(tableSorter.indexFlag, tableSorter.sortingType, "tableApi._jsonObj.data");

					tableApi.genTable(tableApi._attr);

					if(tableSorter.sortFlag)
						tableSorter.drawBorder(tableSorter.indexFlag);
				}
		});
	},

	genDataRawEditElement : function(_dataItemObj, _dataArray) {
		var dataColCount = _dataArray.length;
		var dataRowCount = _dataItemObj.length;
		for(var i = 0; i < dataColCount; i += 1) {
			for(var j = 0; j < dataRowCount; j += 1) {
				//gen edit element
				var $editFrameHtml = $("<div>");
				$editFrameHtml
					.addClass("edit-mode")
					.css({"display":'none', "width":"100%"});

				var editMode = _dataItemObj[j].editMode;
				var $editItemHtml = "";
				switch(editMode) {
					case "text" :
						$editItemHtml = tableApi.genEditRuleText(_dataItemObj[j], _dataArray, i, j);
						break;
					case "select" :
						$editItemHtml = tableApi.genEditRuleSelect(_dataItemObj[j], _dataArray, i, j);
						break;
					case "pureText" :
						$editItemHtml = $("<div>").html(_dataArray[i][j]);
						break;
					case "callBack" :
						$editItemHtml = $("<div>").html(_dataArray[i][j]);
						break;
				}
				$editItemHtml.appendTo($editFrameHtml);
				$("tr[row_tr_idx='" + i + "']").find($("td[row_td_idx='" + j + "']")).append($editFrameHtml);

				//register click event
				switch(editMode) {
					case "text" :
					case "select" :
					case "pureText" :
						$("tr[row_tr_idx='" + i + "']").find($("td[row_td_idx='" + j + "']")).click(
							function() {
								if($(".row_tr").children().find(".hint").length != 0) {
									return false;
								}
								event.stopPropagation();
								var id = "";
								if($(this).find(".dataEdit").length == 0) {
									id = $(this).parent().find(".dataEdit:first")[0].id;
								}
								else {
									id = $(this).find($(".dataEdit"))[0].id;
								}

								if($('body').find('.row_tr').hasClass("data_raw_editing")) {	
									$('body').find('.row_tr').removeClass("data_raw_editing");
									$('body').find('.row_tr').find('.edit-mode').css({"display":'none'});
									$('body').find('.row_tr').find('.static-text').css({"display":''});

								}
								$("#" + id).closest(".row_tr").addClass("data_raw_editing");
								$("#" + id).closest(".row_tr").find('.edit-mode').css({"display":''});
								$("#" + id).closest(".row_tr").find('.static-text').css({"display":'none'});
								$("#" + id).focus();

							}
						);
						break;
					case "callBack" : 
						var _callBackFun = _dataItemObj[j].callBackFun;
						$("tr[row_tr_idx='" + i + "']").find($("td[row_td_idx='" + j + "']")).click(
							function() {
								if($(".row_tr").children().find(".hint").length != 0) {
									return false;
								}
								_callBackFun($(this));
							}
						);
						break;
				}
				

			}
		}
	},

	genEditRuleText : function(_dataObj, _dataArray, _colIdx, _rowIdx) {
		var $editItemHtml = "";
		$editItemHtml = $('<input/>');
		$editItemHtml.addClass("valueMust inputText dataEdit");
		$editItemHtml.attr({"id" : "edit_item_" + _colIdx + "_" + _rowIdx});
		$editItemHtml.attr({"type" : "text", "autocorrect" : "off", "autocapitalize" : "off"});
		$editItemHtml.val(_dataArray[_colIdx][_rowIdx]);
		if(_dataObj.hasOwnProperty("maxlength")) {
			$editItemHtml.attr({"maxlength" : _dataObj.maxlength});
		}

		if(_dataObj.hasOwnProperty("valueMust")) {
			$editItemHtml.removeClass("valueMust");
		}

		if(_dataObj.hasOwnProperty("validator")) {
			if(tableValidator[_dataObj.validator]["keyPress"]) {
				$editItemHtml.keypress(
					 function() {
						return tableValidator[_dataObj.validator]["keyPress"]($(this), event);
					}
				);
			}
			if(tableValidator[_dataObj.validator]["blur"]) {
				$editItemHtml.blur(
					 function() {
					 	var validFlag = tableValidator[_dataObj.validator]["blur"]($(this));
					 	if(validFlag) {
					 		var validDuplicateFlag = true;
							if(tableApi._attr.hasOwnProperty("ruleDuplicateValidation") && (tableApi._attr.ruleDuplicateValidation != "")) {
								var currentEditRuleArray = tableValid_getCurrentEditRuleArray($(this),  tableApi._attr.data);
								var filterCurrentEditRuleArray = tableValid_getFilterCurrentEditRuleArray($(this),  tableApi._attr.data);

								validDuplicateFlag = tableRuleDuplicateValidation[tableApi._attr.ruleDuplicateValidation](currentEditRuleArray, filterCurrentEditRuleArray);
								if(!validDuplicateFlag) {
									tableApi.showHintMsg($(this), "<#JS_duplicate#>");
									return false;
								}
							}
							var ruleValidationFlag = "";
							if(tableApi._attr.hasOwnProperty("ruleValidation") && (tableApi._attr.ruleValidation != "")) {
								var currentEditRuleArray = tableValid_getCurrentEditRuleArray($(this),  tableApi._attr.data);
								ruleValidationFlag = tableRuleValidation[tableApi._attr.ruleValidation](currentEditRuleArray);
								if(ruleValidationFlag != HINTPASS) {
									tableApi.showHintMsg($(this), ruleValidationFlag);
									return false;
								}
							}
					 		tableApi.updateData($(this));
					 	}
					}
				);
			}
		}

		return $editItemHtml
	},

	genEditRuleSelect : function(_dataObj, _dataArray, _colIdx, _rowIdx) {
		var $editItemHtml = "";
		$editItemHtml = $('<select/>');
		$editItemHtml.attr({"id" : "edit_item_" + _colIdx + "_" + _rowIdx});
		$editItemHtml.addClass("inputSelect dataEdit");
		if(_dataObj.hasOwnProperty("option")) {
			for (var optIdx in _dataObj.option) {
				var optionText = optIdx;
				var optionValue =_dataObj.option[optIdx];
				var $optionHtml = $('<option/>');
				$optionHtml.attr("value", optionValue).text(optionText);
				$editItemHtml.append($optionHtml);
			}
		}
		$editItemHtml.val(_dataArray[_colIdx][_rowIdx]);
		$editItemHtml.change(
			function() {
				var validDuplicateFlag = true;
				if(tableApi._attr.hasOwnProperty("ruleDuplicateValidation") && (tableApi._attr.ruleDuplicateValidation != "")) {
					var currentEditRuleArray = tableValid_getCurrentEditRuleArray($(this),  tableApi._attr.data);
					var filterCurrentEditRuleArray = tableValid_getFilterCurrentEditRuleArray($(this),  tableApi._attr.data);

					validDuplicateFlag = tableRuleDuplicateValidation[tableApi._attr.ruleDuplicateValidation](currentEditRuleArray, filterCurrentEditRuleArray);
					if(!validDuplicateFlag) {
						alert("<#JS_duplicate#>");
						return false;
					}
				}
				tableApi.updateData($(this));
			}
		);
		return $editItemHtml
	},

	updateData : function($obj) {
		var eleID = $obj.attr('id');
		var eleValue = $obj.val();
		var colIdx = eleID.replace("edit_item_", "").split("_")[0];
		var rowIdx = eleID.replace("edit_item_", "").split("_")[1];


		tableApi._attr.data[colIdx][rowIdx] = eleValue;

		$("#" + eleID).parent().prev().html(eleValue)
	},

	genDataRawDel : function() {
		if(tableApi._attr.capability.del) {
			var $titleHtml = $("<th>").html("<#CTL_del#>");
			$titleHtml.attr("width", "10%");
			$("#" + tableApi._attr.container).find(".row_title").append($titleHtml);
			
			var $dataHtml = $("<td>");
			$dataHtml.attr("width", "10%");
			var $editHtml = $('<input/>');
			$editHtml.addClass("remove_btn");
			$editHtml.click(
				function() {
					event.stopPropagation();
					var delRawIdx = $(this).closest("*[row_tr_idx]").attr( "row_tr_idx" );
					tableApi._attr.data.splice(delRawIdx,1);
					if(tableSorter.sortFlag)
						tableSorter.sortData(tableSorter.indexFlag, tableSorter.sortingType, "tableApi._jsonObj.data");

					tableApi.genTable(tableApi._attr);

					if(tableSorter.sortFlag)
						tableSorter.drawBorder(tableSorter.indexFlag);
				}
			);
			$editHtml.appendTo($dataHtml);

			$("#" + tableApi._attr.container).find(".row_tr").append($dataHtml);
		}
	},

	//hover start
	setHover : function($elm, _hoverFlag) {
		if(_hoverFlag) {
			var $trObj = $elm.find(".data_tr");
			$trObj.hover(function () {
				tableApi.setClass($(this) , "data_tr_hover");
			}, function () {
				tableApi.unsetClass($(this) , "data_tr_hover");
			});
		}
	},

	classNameToList : function (className) {
		return className.replace(/^\s+|\s+$/g, '').split(/\s+/);
	},

	// The className parameter (str) can contain multiple class names separated by whitespace
	setClass : function ($elm, className) {
		var classList = tableApi.classNameToList(className);
		for (var i = 0; i < classList.length; i += 1) {
			if (!$elm.hasClass(classList[i])) {
				$elm.addClass( classList[i] );
			}
		}
	},

	// The className parameter (str) can contain multiple class names separated by whitespace
	unsetClass : function ($elm, className) {
		var classList = tableApi.classNameToList(className);
		for (var i = 0; i < classList.length; i += 1) {
			var repl = new RegExp(
				'^\\s*' + classList[i] + '\\s*|' +
				'\\s*' + classList[i] + '\\s*$|' +
				'\\s+' + classList[i] + '(\\s+)',
				'g'
			);
			$elm.removeClass(classList[i]);
		}
	},
	//hover end

	genDivisionLine : function() {
		var $divisionLineHtml = $("<div>");
		$divisionLineHtml.addClass("divisionLine");
		return $divisionLineHtml;
	},

	removeElement : function(_className) {
		if($("." + _className).length) {
			$("." + _className).remove();
		}
	},

	transformSelectOptionToText : function(_optionObj, _value) {
		var transformText = _value;
		for(var i in _optionObj){
			if(_optionObj.hasOwnProperty(i)) {
				if(_optionObj[i] == _value) {
					transformText = i;
					break;
				}
			}
		}
		return transformText;
	},
	showHintMsg : function(_$obj, _hintMsg) {
		var $hintHtml = $('<div>');
		$hintHtml.addClass("hint");
		$hintHtml.html(_hintMsg);
		_$obj.after($hintHtml);
		_$obj.focus();
	}
}


