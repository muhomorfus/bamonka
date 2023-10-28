<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <h1>{{ team.name }}</h1>

            <p class="text-muted mb-0">{{ team.description }}</p>
          </div>
        </div>

        <ProfileShort v-for="m in teamMembers" :key="m.id" :profile="{'name': m.name, 'nickname': m.nickname, 'working': m.working}"/>

      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import ProfileShort from "@/components/ProfileShort.vue";

export default defineComponent({
  components: {ProfileShort},
  methods: {
    ...mapActions(['loadTeam', 'loadTeamMembers']),
  },
  created() {
    this.loadTeam(this.$route.params.id.toString())
    this.loadTeamMembers(this.$route.params.id.toString())
  },
  computed: {
    ...mapGetters(['team', 'teamMembers']),
  },
});
</script>