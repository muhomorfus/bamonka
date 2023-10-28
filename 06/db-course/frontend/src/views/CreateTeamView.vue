<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <h1>Creating team</h1>

            <form @submit.prevent="createTeam">
              <div class="mb-3">
                <label for="name" class="form-label">Name</label>
                <input type="text" class="form-control" id="name"
                       placeholder="Super-puper team" v-model="team.name">
              </div>
              <div class="mb-3">
                <label for="description" class="form-label">Description</label>
                <textarea class="form-control" id="description" rows="3" v-model="team.description"></textarea>
              </div>

              <button class="btn btn-primary" type="submit">Create</button>
            </form>
          </div>
        </div>

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
    ...mapActions(['createTeam']),
  },
  computed: {
    ...mapGetters(['team']),
  },
});
</script>