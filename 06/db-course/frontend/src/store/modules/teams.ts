import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadTeams(ctx: any) {
            try {
                const resp = await API.getTeamList()
                ctx.commit('saveTeams', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async deleteTeam(ctx: any, id: string) {
            try {
                await API.deleteTeam(id)
                ctx.commit('removeTeam', id)
                ctx.dispatch('showError', 'Team successfully deleted.')
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },
    },

    mutations: {
        saveTeams(state: any, teams: Array<openapi.Team>) {
            state.teams = teams
        },

        removeTeam(state: any, id: string) {
            state.teams = state.teams.filter((t: openapi.Team) => t.id != id)
        }
    },

    state: {
        teams: Array<openapi.Team>(),
    },

    getters: {
        teams(state: any) {
            return state.teams
        },
    },
};