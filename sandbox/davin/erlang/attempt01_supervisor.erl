-module(attempt01_supervisor).
-behaviour(supervisor).

-export([start/0, start_in_shell_for_testing/0, start_link/1, init/1]).

% Standard boilerplate for start/0.
start() ->
    spawn(fun() ->
        supervisor:start_link({local, ?MODULE}, ?MODULE, _Arg = [])
        end).

% For interactive testing purposes only.
start_in_shell_for_testing() ->
    {ok, Pid} = supervisor:start_link({local, ?MODULE}, ?MODULE, _Arg = []),
    % Prevent termination of the shell in erl from terminating this supervisor.
    unlink(Pid).

% Standard boilerplate for start_link/1.
start_link(Args) ->
    supervisor:start_link({local, ?MODULE}, ?MODULE, Args).

% Starts up the explicitly named pullers as children.
init([]) ->
    io:format("DBG Info: ~p starting~n", [?MODULE]),
    {ok,
        {
            {simple_one_for_one, 40, 10},
            [
                {
                    attempt01_drone,
                    {attempt01_server, start_link, []},
                    transient,
                    5000,
                    worker,
                    [attempt01_server]
                }
            ]
        }
    }.

