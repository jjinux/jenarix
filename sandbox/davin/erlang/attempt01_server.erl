% Copyright (c) 2009, DeLano Scientific LLC, Palo Alto, California, USA.
% All rights reserved.

% Redistribution and use in source and binary forms, with or without
% modification, are permitted provided that the following conditions are
% met:
%
% * Redistributions of source code must retain the above copyright
%   notice, this list of conditions and the following disclaimer.
%
% * Redistributions in binary form must reproduce the above copyright
%  notice, this list of conditions and the following disclaimer in the
%  documentation and/or other materials provided with the distribution.
%
% * Neither the name of the DeLano Scientific LLC nor the names of its
%  contributors may be used to endorse or promote products derived from
%  this software without specific prior written permission.
%
% THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
% "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
% LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
% A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
% HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
% SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
% LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
% DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
% THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
% (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
% OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-module(attempt01_server).
-behaviour(gen_server).


%% API public exports
-export([start_link/4]).

%% gen_server callbacks public exports
-export([init/1, handle_call/3, handle_cast/2, handle_info/2,
         terminate/2, code_change/3]).

-record(state, {meaty_function, outport_connects_to_list, 
                errport_connects_to_list}).


%%====================================================================
%% API implementation
%%====================================================================
%%--------------------------------------------------------------------
%% Function: start_link() -> {ok,Pid} | ignore | {error,Error}
%% Description: Starts the server
%%--------------------------------------------------------------------
start_link(Uid, MeatFuncType, OutportConns, ErrportConns) ->
    % Conceive of a unique name to register this instance as.
    gen_server:start_link({local, Uid}, ?MODULE, 
                          [MeatFuncType, OutportConns, ErrportConns], []).


%%====================================================================
%% gen_server callbacks implementation
%%====================================================================
%%--------------------------------------------------------------------
%% Function: init(Args) -> {ok, State} |
%%                         {ok, State, Timeout} |
%%                         ignore |
%%                         {stop, Reason}
%% Description: Initiates the server
%%--------------------------------------------------------------------
init([MeatFuncType, OutportConns, ErrportConns]) ->
    {ok, #state{meaty_function=MeatFuncType,
                outport_connects_to_list=OutportConns,
                errport_connects_to_list=ErrportConns}}.

%%--------------------------------------------------------------------
%% Function: handle_call(Request, From, State) ->
%%                  {reply, Reply, State} |
%%                  {reply, Reply, State, Timeout} |
%%                  {noreply, State} |
%%                  {noreply, State, Timeout} |
%%                  {stop, Reason, Reply, State} |
%%                  {stop, Reason, State}
%% Description: Handling (sync) call messages
%%--------------------------------------------------------------------
handle_call(_Request, _From, State) ->
    Reply = ok,
    {reply, Reply, State}.

%%--------------------------------------------------------------------
%% Function: handle_cast(Msg, State) -> {noreply, State} |
%%                                      {noreply, State, Timeout} |
%%                                      {stop, Reason, State}
%% Description: Handling (async) cast messages
%%--------------------------------------------------------------------
handle_cast(Msg, State) when State#state.meaty_function==m_adds_one ->
    Result = m_adds_one(Msg),
    erlang:display("DBG AddsOne result: " ++ integer_to_list(Result)),
    [gen_server:cast(Uid, Result) || Uid <- State#state.outport_connects_to_list],
    {noreply, State};
handle_cast(Msg, State) when State#state.meaty_function==m_negate ->
    Result = m_negate(Msg),
    erlang:display("DBG Negate result: " ++ integer_to_list(Result)),
    [gen_server:cast(Uid, Result) || Uid <- State#state.outport_connects_to_list],
    {noreply, State};
handle_cast(Msg, State) when State#state.meaty_function==m_doubles ->
    Result = m_doubles(Msg),
    erlang:display("DBG Doubles result: " ++ integer_to_list(Result)),
    [gen_server:cast(Uid, Result) || Uid <- State#state.outport_connects_to_list],
    {noreply, State};
handle_cast(_Msg, State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% Function: handle_info(Info, State) -> {noreply, State} |
%%                                       {noreply, State, Timeout} |
%%                                       {stop, Reason, State}
%% Description: Handling all non call/cast messages
%%--------------------------------------------------------------------
handle_info(_Info, State) ->
    {noreply, State}.

%%--------------------------------------------------------------------
%% Function: terminate(Reason, State) -> void()
%% Description: This function is called by a gen_server when it is about to
%% terminate. It should be the opposite of Module:init/1 and do any necessary
%% cleaning up. When it returns, the gen_server terminates with Reason.
%% The return value is ignored.
%%--------------------------------------------------------------------
terminate(_Reason, _State) ->
    ok.

%%--------------------------------------------------------------------
%% Function: code_change(OldVsn, State, Extra) -> {ok, NewState}
%% Description: Convert process state when code is changed
%%--------------------------------------------------------------------
code_change(_OldVsn, State, _Extra) ->
    {ok, State}.


%%====================================================================
%% Internal functions implementation
%%====================================================================

m_adds_one(Request) ->
    1 + Request.
m_negate(Request) ->
    -1 * Request.
m_doubles(Request) ->
    2 * Request.
