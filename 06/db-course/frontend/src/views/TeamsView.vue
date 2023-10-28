<template>
  <main class="container">
      <br>

    <div class="card-header d-flex justify-content-between">
      <div>
        <h1>Teams</h1>
      </div>
      <div>
        <a v-if="isAdmin" class="btn btn-primary" href="/teams/create">New</a>
      </div>
    </div>

      <div class="row">

        <div v-for="t in teams" :key="t.id" class="col-md-6">
          <div class="card mt-4">
            <div class="card-body">
              <h4>
                {{ t.name }}
              </h4>

              <p class="text-muted">
                {{ t.description }}
              </p>

              <a class="card-link" :href="`/teams/${t.id}`">View</a>
              <a v-if="isAdmin" class="card-link" :href="`/teams/${t.id}/edit`">Edit</a>
              <a v-if="isAdmin" href="#" class="card-link" @click.prevent="deleteTeamLocal(t.id)">Delete</a>
              </div>
            </div>
          </div>
      </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";

export default defineComponent({
  methods: {
    ...mapActions(['loadTeams', 'deleteTeam']),
    deleteTeamLocal(id: string) {
      this.deleteTeam(id)
    }
  },
  created() {
    this.loadTeams()
  },
  computed: {
    ...mapGetters(['teams', 'isAdmin']),
  },
});
</script>