#######################################################
#
# 后台动态库 Makefile 的写法
#
########################################################

# 包含基本的系统参数定义【不能更改】
include $(OB_REL)/etc/NGbasedefine.mk

########################################################

# 目标名称，最终的程序文件名是 lib$(DEST)$(DLLTAIL)【必须修改】
DEST = server

# 目标的类型，必须是APP 
DEST_TYPE = BIN


# 编译目标程序需要的源代码文件，可以带路径（但必须是绝对路径）【必须修改】
DEST_SOURCES = server.cpp
# SDL_DBCONN = SHDB_AD
# 目标的头文件安装路径，最终目录将是 $OB_REL/include/$(SUBSYSTEM)/$(HEADER_PATH)/ 【可选】
#HEADER_PATH = 

# 需要安装的头文件，文件名需带路径【可选】
#HEADERS = ./*.h


#IMPL_SDL=abm_interface_budget.sdl

# CALLED_SDL=abm_interface_def.sdl abm_rdl_queue_def.sdl abm_interface_budget_def.sdl abm_rdl_billing_def.sdl abm_interface_asset_upload_db.sdl

#IMPL_RDL=abm_mdb_serv_queue.rdl 
USER_SDL_INC_PATH = ../../idl  $(OB_REL)/idl/balance
#USER_RDL_INC_PATH = $(OB_REL)/rdl/balance
# 定义测试程序名称【可选】
#TEST_PROGRAM = test

# 定义测试程序的源代码文件【可选】
#TEST_SOURCES = test.cpp

# 其它选项【可选】
#IS_OPENBROKER_SOURCE = 1
NEED_DATABASE = 1
# SELFDEF_SDL_INSTALL_PATH = $(OB_REL)/idl/balance/

#单元测试相关设置
#UNITTEST_TOTAL_ON = 1
NEED_UNITTEST = 1
NEED_COVERTEST = 1
NEED_AUTO_UNITTEST_SOURCES=1

# 不需要生成单元测试的源代码文件列表【方案1】
UNCPP2UNIT_SOURCES = 


# 过滤掉UNCPP2UNIT_SOURCES源代码文件，剩下需要编译的测试源代码文件列表【可选】
UNITTEST_SOURCES = 

# ADDTIONAL_TEST_LIBS = base_ruler$(BUILDTYPE)            \
#                       public_common_base$(BUILDTYPE)   \
#                       app_config_test_lib$(BUILDTYPE)
	
#NEED_MIDDLE_WARE = 0
#NEED_OPENBROKER = 0
#ADDTIONAL_INSTALL_HEADER_CMD = mkdir -p $(OB_REL)/include/balance/ && cp -rf abm_interface_budget_sdl_i.h abm_interface_budget_sdl_s.h $(OB_REL)/include/balance/
#ADDTIONAL_INSTALL_IDL_CMD = mkdir -p $(OB_REL)/idl/rating_billing/abm && cp -rf idl/*.sdl $(OB_REL)/idl/rating_billing/abm
#SELFDEF_SDL_INSTALL_PATH = $(OB_REL)/idl/balance 
#ADDTIONAL_INSTALL_HEADER_CMD = mkdir -p $(OB_REL)/idl/balance && cp -rf idl/*.sdl $(OB_REL)/idl/balance \
#				&& chmod 777 $(OB_REL)/idl/balance/abm_interface_billing.sdl
#IS_TARGET_MW_SENSETIVE=1

#NEED_ERRCFG=1
NEED_ERRCFG=0
ERRCFG_DEF=MODULE_test_DEF
ERRCFG_XML=error_config.xml

########################################################
# 用户的宏定义，为编译器添加其它的 -D 参数，注意不要自行添加 -D 【可选】
#USER_DEFS = HAS_NO_MALLINFO DEBUG_REAL_MESSAGE _UNIT_TEST_
USER_DEFS=TEST_LOG TEST_CONFIG TEST_PROPERTY  

USER_INC_PATH = .\
	$(OB_REL)/include \
	$(OB_REL)/include/public  \
	$(OB_REL)/include/public/common/base \
	$(OB_REL)/include/public/common/log \
	$(OB_REL)/include/3rd/TinyXml	\
	$(OB_REL)/include/balance\
	$(OB_REL)/include/balance_domestic \
	$(OB_REL)/include/public/xc \
	$(OB_REL)/include/public/sal \
	$(OB_REL)/include/public/ob_kernel\
	$(OB_REL)/include/public/common/estat \
	$(OB_REL)/include/3rd/otl	\
	$(OB_REL)/include/billing\
	$(OB_REL)/include/rating_billing/user_mdb

# 需要连接的其它库文件，应使用 $(BUILDTYPE) 作为后缀【可选】
USER_LIBRARIES = TinyXml commlog$(BUILDTYPE) ailog_interface$(BUILDTYPE) \
		public_app_config$(BUILDTYPE) \
		public_sal$(BUILDTYPE) SdlBuffer$(BUILDTYPE)\
		abm_mdb_balance_rdl$(BUILDTYPE) \
		base_ruler$(BUILDTYPE) \
		public_common_base$(BUILDTYPE)   \
		app_config_test_lib$(BUILDTYPE) \
		public_common_dbapix$(BUILDTYPE)
########################################################
include $(OB_REL)/etc/NGCmake
